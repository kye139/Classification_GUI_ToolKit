import tensorflow as tf
import numpy as np
import struct
import threading
import math

class NetClassfierGenerator:
    def __error__(self,errordata):#에러 핸들링 함수
        print(errordata)
        exit(1)
    
    def __init__(self,payload):#소켓으로 들어온 바이트 배열을 입력으로 받는다. 
        self.sess = tf.Session()#
        self.originalPayload = payload.copy()
        self.remainPayload = payload
        self.inputLayer = None#입력층
        self.flow = None#전체 신경망의 흐름을 나타내는 변수
        self.trainLayer = None#훈련에 대한 노드
        self.loss = None#오차에 대한 노드
        self.accuracy = None#정확도에 대한 노드
        self.labelPlaceholder = None#레이블에 대한 노드
        self.isOneHot = None#원핫 인코딩인지 아닌지 여부
        self.b_trainingPlaceholder = tf.placeholder_with_default(False,shape=())
        #훈련시에만 드롭아웃을 적용하기 위함
        self.n_in = None #신경망을 돌면서 텐서의 가로 또는 새로 차원을 나타내는 변수
        self.li_shape = None #1월 16일 n_in을 사용하지 말고 그냥 전체 텐서의 shape을 계산하면서 가자.
        #li_shape은 합성곱, 완전연결계층 등의 차원 문제가 발생하는 모든 곳에서 수정되어 나가야 한다.
        #그러나 이 변수는 절대로 private 함수 내에서는 값이 수정되도록 하지 않도록 하자.
        #일반적으로 [배치크기, 가로, 세로, 채널수]또는 [배치크기, 1차원으로 늘린 차원수]

    def __makeInputLayer__(self,n_features,inputlayer,flow):
        '''
        입력층을 생성하는 함수로 인자로 특징의 갯수, 입력층을 나타내는 텐서, 흐름을 따라가는 텐서로 이루어짐
        '''
        inputlayer = tf.placeholder(tf.float32,shape = (None,n_features))
        flow = inputlayer


        return flow
    
    def __makeConvLayer__(self,n_initMethod,f_mean,f_std,n_filterSize,n_strides,n_padding,n_neuron,f_bias,flow):
        '''
        합성곱 계층을 생성하는 함수이다. 
        n_in에 대한 입력이 필요하다.
        '''
        filter,bias,padding_method = None,None,None

        if n_initMethod == 0:#절단정규분포인경우, 기본값
            filter = tf.Variable(tf.truncated_normal([n_filterSize,n_filterSize,self.li_shape[3],n_neuron],mean=f_mean,stddev=f_std,dtype=tf.float32))
        elif n_initMethod == 1:#일반정규분포사용
            filter = tf.Variable(tf.random_normal([n_filterSize,n_filterSize,self.li_shape[3],n_neuron],mean=f_mean,stddev=f_std,dtype=tf.float32))
        else:
            self.__error__('컨볼루션계층에서 n_initMeghod 에러')
            
        if n_padding == 0:#제로패딩
            s_padding_method = 'SAME'
        elif n_padding == 1:#패딩없음
            s_padding_method = 'VALID'
        else:
            self.__error__('컨볼루션계층에서 padding_method 에러')
            
        bias = tf.Variable(tf.constant(f_bias,shape=[n_neuron]))

        flow = tf.nn.conv2d(flow,filter,strides=[1,n_strides,n_strides,1],padding = s_padding_method)

        flow = flow + bias

        return flow

    def __makePoolingLayer__(self,n_filterSize,n_strides,n_padding,n_poolingMethod,flow):
        '''
        풀링 계층을 생성하는 함수이다.
        '''
        s_padd = None
        if n_padding == 0:
            s_padd = "SAME"
        elif n_padding == 1:
            s_padd = "VALID"
        else:
            self.__error__("풀링계층의 패딩에서 오류")

        if n_poolingMethod == 0: #기본값 재로패딩사용
            flow = tf.nn.max_pool(flow,ksize = n_filterSize,strides = n_strides,padding = s_padd)
        elif n_poolingMethod == 1: #평균풀링
            flow = tf.nn.avg_pool(flow,ksize = n_filterSize,strides = n_strides,padding = s_padd)



        return flow

    def __makeDenseLayer__(self,n_initMethod,f_mean,f_std,n_neuron,f_bias,flow,n_in):
        '''
        완전연결 계층을 구성하는 함수이다.
        '''
        weight,bias = None,None

        if n_initMethod == 0:#절단정규분포 기본값
            weight = tf.Variable(tf.truncated_normal([n_in,n_neuron],mean = f_mean,stddev = f_std))
        elif n_initMethod == 1:#일반정규분포
            weight = tf.Variable(tf.truncated_normal([n_in,n_neuron],mean = f_mean,stddev = f_std))
        elif n_initMethod == 2:#He 초기화
            pass
        elif n_initMethod == 3:#Xavier 초기화
            pass
        else:
            self.__error__("완전연결계층에서 오류")
        
        flow = tf.matmul(flow,weight)

        bias = tf.Variable(tf.constant(f_bias,shape=[n_neuron]))

        flow = flow + bias
        
        return flow
        
    def __makeActivationLayer__(self,n_activationMethod,flow):

        if n_activationMethod == 0:#기본값 ReLu
            flow = tf.nn.relu(flow)
        elif n_activationMethod == 1:#시그모이드
            flow = tf.nn.sigmoid(flow)
        elif n_activationMethod == 2:
            flow = tf.nn.tanh(flow)
        elif n_activationMethod == 3:
            flow = tf.nn.leaky_relu(flow)
        else:
            self.__error__("활성화계층에서 오류")

        return flow

    def __makeBatchNormLayer__(self,f_momentumParm,flow):
        '''
        아직 배치정규화는 구현되지 않았다. 
        '''

        return flow

    def __makeDropoutLayer__(self,f_dropoutRatio,flow):
        #flow = tf.nn.dropout(flow,f_dropoutRatio)
        flow = tf.layers.dropout(flow,f_dropoutRatio,training=self.b_trainingPlaceholder)

        return flow

    def __makeSoftmaxLayer__(self,n_onehotMethod,n_label,flow):
        #n_label은 분류 데이터의 갯수 
        
        correct = tf.nn.in_top_k(flow,self.labelPlaceholder,1)

        if n_onehotMethod == 0: #기본값. 원핫 인코딩이 아닌경우
            self.labelPlaceholder = tf.placeholder(tf.int32,shape=(None))
            self.isOneHot = False
            flow = tf.nn.sparse_softmax_cross_entropy_with_logits(flow,self.labelPlaceholder)
           
        if n_onehotMethod == 1:#원핫 인코딩 벡터인 경우
            self.labelPlaceholder = tf.placeholder(tf.float32,shape=(None,n_label))
            self.isOneHot = True
            flow = tf.nn.softmax_cross_entropy_with_logits(flow,self.labelPlaceholder)
        
        self.loss = tf.reduce_mean(flow)
        
        self.accuracy = tf.reduce_mean(tf.cast(correct,tf.float32))
        flow = self.loss
        return flow

    def __makeOptimizerLayer__(self,n_optimizer,f_momentum,f_learningRate,flow):
        optimizer = None
        if n_optimizer == 0: #기본값 경사하강법
            optimizer = tf.train.GradientDescentOptimizer(f_learningRate)
        elif n_optimizer == 1:
            optimizer = tf.train.MomentumOptimizer(f_learningRate,momentum = f_momentum)
        elif n_optimizer == 2:
            optimizer = tf.train.MomentumOptimizer(f_learningRate,momentum = f_momentum,use_nesterov=True)
        elif n_optimizer == 3:
            optimizer = tf.train.RMSPropOptimizer(f_learningRate,momentum = 0.9,decay = 0.9,epsilon = 1e-10)
        elif n_optimizer == 4:#아담 옵티마이저의 경우 기본 학습률을 0.001로 설정해보자
            optimzier = tf.train.AdamOptimizer(f_learningRate)
        self.trainLayer = optimizer.minimize(flow)

        return flow
    
    

    def interpretPayload(self):
        count = 0
        prevcode = 0
        while True:
            code = struct.unpack('1i',self.remainPayload[:4])
            code = int(code[0])#어떤 층인지 파악하기 위함
            self.remainPayload = self.remainPayload[4:]

            if code == -1: #입력층인경우
                data = struct.unpack('1i',self.remainPayload[:4])
                self.remainPayload = self.remainPayload[4:]

                n_features = int(data[0])
                '''
                self.n_in = int(n_features**0.5)#sqrt(n_features)
                if self.n_in**2 != n_features:
                    self.__error__("입력층에서 차원에러! n_in**2 != n_features 가 False")
                '''    

                self.flow = self.__makeInputLayer__(n_features,self.inputLayer,self.flow)
                self.li_shape = self.flow.get_shape()


            elif code == -2:#합성곱 계층

                if prevcode == -1 or prevcode == -4:#이전 계층이 입력층이거나 완전연결계층이면 reshape과정을 거쳐야함
                    #즉 이 if문은 평탄화 되어 있는 벡터에 대한 처리 과정을 나타낸다. 
                    #원본 이미지가 3채널인것으로 가정한다. (BGR)
                    #self.n_in이 가로 또는 새로의 차원 정보를 가지고 있을것이다.
                    #code가 -1인 입력층인경우에서 n_in을 초기화 했을 것.
                    #지금은 완전연결계층에서 다시 합성곱으로 간 경우는 생각하지 않았다. 
                    n_features = self.li_shape[1] #가령 가로4 세로 4 채널3의 이미지의 경우 이 값은 4*4*3일것
                    n_channel = 3
                    n_width = int((n_features/3)**0.5)#n_width의 값이 4가 되도록 하는 것 
                    if n_width*n_width*3 != n_features:
                        self.__error__('합성곱계층 구성중 채널 오류!')

                    self.flow = tf.reshape(self.flow,shape=[-1,n_width,n_width,n_channel])

                data = struct.unpack('1i2f4i1f',self.remainPayload[:24])
                self.remainPayload = self.remainPayload[24:]

                n_initMethod, f_mean   , f_std     = int(data[0]), float(data[1]), float(data[2])
                n_filterSize, n_strides, n_padding = int(data[3]), int(data[4])  , int(data[5])
                n_neuron    , f_bias               = int(data[6]), float(data[7])

                self.flow = self.__makeConvLayer__(n_initMethod,f_mean,f_std,\
                n_filterSize,n_strides,n_padding,n_neuron,f_bias,self.flow)

                self.li_shape = self.flow.get_shape()
'''
                if n_strides == 0:#제로패딩
                    n_outWidth = math.ceil(self.li_shape[1]/n_strides)
                    self.li_shape = [-1,n_outWidth,n_outWidth,n_neuron]
                elif n_strides == 1:#패딩없음
                    n_outWidth = int((self.li_shape[1]-n_filterSize)/n_strides)+1
                    self.li_shape = [-1,n_outWidth,n_outWidth,n_neuron]
'''         
            elif code == -3:#풀링계층
                
                data = struct.unpack('4i',self.remainPayload[:16])
                self.remainPayload = self.remainPayload[16:]

                n_filterSize,n_strides    = int(data[0]),int(data[1])
                n_padding,n_poolingMethod = int(data[2]),int(data[3])

                self.flow = self.__makePoolingLayer__(n_filterSize,n_strides,n_padding,n_poolingMethod,self.flow)
                self.li_shape = self.flow.get_shape()
'''
                if n_padding == 0:#제로패딩 적용
                    n_outWidth = math.ceil(self.li_shape[1]/n_strides)
                    n_neuron = self.li_shape[3]
                    self.li_shape = [-1,n_outWidth,n_outWidth,n_neuron]
                elif n_padding == 1:#패딩 미적용
                    n_outWidth = int((self.li_shape[1] - n_filterSize)/n_strides)+1
                    n_neuron = self.li_shape[3]
                    self.li_shape = [-1,n_outWidth,n_outWidth,n_neuron]
                else:
                    self.__error__('풀링층에서 차원 에러!')
'''


            elif code == -4:#완전연결계층
                if prevcode == -1:#이전층이 합성곱 계층이면 평탄화 과정이 필요하다.
                    tu_prevshape = self.li_shape
                    r,c,chan = tu_prevshape[1],tu_prevshape[2],tu_prevshape[3]
                    self.flow = tf.reshape(self.flow,[-1,r*c*chan])#flatten

                data = struct.unpack('1i2f1i1f',self.remainPayload[:20])
                self.remainPayload = self.remainPayload[20:]

                n_initMethod,f_mean   = int(data[0]),float(data[1])
                f_std,n_neuron,f_bias = float(data[2]),int(data[3]),float(data[4])

                self.flow = self.__makeDenseLayer__(n_initMethod,f_mean,f_std,n_neuron,f_bias,self.flow)

                self.li_shape = self.flow.get_shape()

            elif code == -5:#활성화 계층
                data = struct.unpack('1i',self.remainPayload[:4])
                self.remainPayload = self.remainPayload[4:]

                n_activationMethod = int(data[0])

                self.flow = self.__makeActivationLayer__(n_activationMethod,self.flow)


            elif code == -6:#배치정규화계층
                data = struct.unpack('1i',self.remainPayload[:4])
                self.remainPayload = self.remainPayload[4:]

                f_momentumParm = float(data[0])

                self.flow = self.__makeBatchNormLayer__(f_momentumParm,self.flow)



            elif code == -7:#드롭아웃계층
                data = struct.unpack('1i',self.remainPayload[:4])
                self.remainPayload = self.remainPayload[4:]

                f_dropoutRatio = float(data[0])

                self.flow = self.__makeDropoutLayer__(f_dropoutRatio,self.flow)



            elif code == -8:#분류계층
                data = struct.unpack('2i',self.remainPayload[:8])
                self.remainPayload = self.remainPayload[8:]

                n_onehotMethod,n_label = int(data[0]),int(data[1])

                self.flow = self.__makeSoftmaxLayer__(n_onehotMethod,n_label,self.flow)


            elif code == -9:#옵티마이저 계층
                data = struct.unpack('1i2f',self.remainPayload[:12])
                self.remainPayload = self.remainPayload[12:]

                n_optimizer = int(data[0])
                f_momentum = float(data[1])
                f_learningRate = float(data[2])

                self.flow = self.__makeOptimizerLayer__(n_optimizer,f_momentum,f_learningRate,self.flow)

            prevcode = code



