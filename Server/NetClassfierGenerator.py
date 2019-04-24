import tensorflow as tf
import numpy as np
import struct
from dictSaver import dictSaver

PAYLOADBIN = 'PayloadBin.txt'


class NetClassfierGenerator:
    '''
    신경망을 구성하는 클래스이다
    생성자에서 클라이언트로부터 받은 바이트 배열을 필요로 한다.
    interpretPyaload 함수를 명시적으로 호출해주어야 내부적으로 신경망이
    신경망이 디스크에 저장된다. 
    '''
    

    def __error__(self,s_errordata):#에러 핸들링 함수
        print(s_errordata)
        exit(1)
    
    def __init__(self,byte_payload):#소켓으로 들어온 바이트 배열을 입력으로 받는다. 
        
        self.byte_originalPayload = byte_payload[:] #아마도 값 복사가 이루어 질것!
        self.remainPayload = byte_payload

        self.tfplc_inputLayer = None#입력층 Placeholder
        self.flow = None#전체 신경망의 흐름을 나타내는 변수
        self.trainLayer = None#훈련에 대한 노드
        self.loss = None#오차에 대한 노드
        self.accuracy = None#정확도에 대한 노드
        self.labelPlaceholder = None#레이블에 대한 노드 
        self.isOneHot = None#원핫 인코딩인지 아닌지 여부
        self.b_trainingPlaceholder = tf.placeholder_with_default(False,shape=(),name='b_trainingPlc')
        #훈련시에만 드롭아웃을 적용하기 위함, 훈련시에만 이 변수에 True 값을 넣어주면 된다. 
        self.tfts_argmax = None
        self.li_shape = None 
        #1월 16일 n_in을 사용하지 말고 그냥 전체 텐서의 shape을 계산하면서 가자.
        #li_shape은 합성곱, 완전연결계층 등의 차원 문제가 발생하는 모든 곳에서 수정되어 나가야 한다.
        #그러나 이 변수는 절대로 private 함수 내에서는 값이 수정되도록 하지 않도록 하자.
        #일반적으로 [배치크기, 가로, 세로, 채널수]또는 [배치크기, 1차원으로 늘린 차원수]
        self.dict_tsinfo = {} #텐서들의 정보를 저장해놓을 딕셔너리
        self.li_code = []     #모든 코드 정보를 저장해둘 리스트 아직은 사용 안한다.

        self.prev_softmax = None #디버깅용
        self.after_softmax = None #디버깅용
            

    def getB_trainingPlaceHolder(self):
        if self.b_trainingPlaceholder is not None:
            return self.b_trainingPlaceholder
        else:
            print('b_trainingPlaceholder가 정의되지 않았다.')

    def getInputPlaceHolder(self):
        if self.tfplc_inputLayer is not None:
            return self.tfplc_inputLayer
        else:
            print('inputLayer가 정의되지 않았다.')

    def getLabelPlaceHolder(self):
        if self.labelPlaceholder is not None:
            return self.labelPlaceholder
        else:
            print('labelPlaceholder가 정의되지 않았다.')
    
    def getTrainTensor(self):
        if self.trainLayer is not None:
            return self.trainLayer
        else:
            print('trainLayer가 정의되지 않았다.')

    def getAccuracyTensor(self):
        if self.accuracy is not None:
            return self.accuracy
        else:
            print('accuracy가 정의되지 않았다.')
    
    def getLossTensor(self):
        if self.loss is not None:
            return self.loss
        else:
            print('loss가 정의되지 않았다.')

    def getArgmaxTensor(self):
        if self.tfts_argmax is not None:
            return self.tfts_argmax
        else:
            print('argmax가 정의되지 않았다.')


    def __makeInputLayer__(self,n_features,inputlayer,flow):
        '''
        입력층을 생성하는 함수로 인자로 특징의 갯수, 입력층을 나타내는 텐서, 흐름을 따라가는 텐서로 이루어짐
        '''

        self.tfplc_inputLayer = tf.placeholder(tf.float32,shape = (None,n_features),name='input')
        flow = self.tfplc_inputLayer

        return flow
    
    def __makeConvLayer__(self,n_initMethod,f_mean,f_std,n_filterSize,n_strides,n_padding,n_neuron,f_bias,flow):
        '''
        합성곱 계층을 생성하는 함수이다. 
        n_in에 대한 입력이 필요하다.
        '''
        filter,bias,s_padding_method = None,None,None

        if n_initMethod == 0:#절단정규분포인경우, 기본값
            filter = tf.Variable(tf.truncated_normal([n_filterSize,n_filterSize,int(self.li_shape[3]),n_neuron],mean=f_mean,stddev=f_std,dtype=tf.float32))
        elif n_initMethod == 1:#일반정규분포사용
            filter = tf.Variable(tf.random_normal([n_filterSize,n_filterSize,int(self.li_shape[3]),n_neuron],mean=f_mean,stddev=f_std,dtype=tf.float32))
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
            flow = tf.nn.max_pool(flow,ksize = (1,n_filterSize,n_filterSize,1),strides = (1,n_strides,n_strides,1),padding = s_padd)
        elif n_poolingMethod == 1: #평균풀링
            flow = tf.nn.avg_pool(flow,ksize = (1,n_filterSize,n_filterSize,1),strides = (1,n_strides,n_strides,1),padding = s_padd)

        return flow

    def __makeDenseLayer__(self,n_initMethod,f_mean,f_std,n_neuron,f_bias,flow):
        '''
        완전연결 계층을 구성하는 함수이다.
        '''
        weight,bias = None,None
        n_in = int(self.li_shape[1])

        if n_initMethod == 0:#절단정규분포 기본값
            weight = tf.Variable(tf.truncated_normal([n_in,n_neuron],mean = f_mean,stddev = f_std))
        elif n_initMethod == 1:#일반정규분포
            weight = tf.Variable(tf.truncated_normal([n_in,n_neuron],mean = f_mean,stddev = f_std))
        elif n_initMethod == 2:#He 초기화
            self.__error__("아직 he 초기화는 구현되지 않았다")
            pass
        elif n_initMethod == 3:#Xavier 초기화
            self.__error__("아직 xavier 초기화는 구현되지 않았다")
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
        elif n_activationMethod == 2:#tanh
            flow = tf.nn.tanh(flow)
        elif n_activationMethod == 3:#leaky relu
            flow = tf.nn.leaky_relu(flow)
        else:
            self.__error__("활성화계층에서 오류")

        return flow

    def __makeBatchNormLayer__(self,f_momentumParm,flow):
        '''
        혹시 이 간단한 구현이 문제가 된다면
        tf.nn.batch_normalization() 함수를 고려해보자.
        이 함수는 훈련할 때는 미니배치 크기만큼, 
        테스트 시에는 전체 데이터셋을 기반으로 평균과 표준편차를
        직접 계산해서 함수에 인자로 넣어주어야 하고 스케일 조정과
        이동을 위한 파라미터를 생성해야 한다.
        혹시 이 구현이 오류가 된다면 인터넷에서 소스코드를 찾아보자.
        '''

        flow = tf.layers.batch_normalization(flow,training = self.b_trainingPlaceholder,momentum=f_momentumParm)

        return flow

    def __makeDropoutLayer__(self,f_dropoutRatio,flow):
        #flow = tf.nn.dropout(flow,f_dropoutRatio)
        print('__makeDropoutLayer__ ',f_dropoutRatio)
        flow = tf.layers.dropout(flow,f_dropoutRatio,training=self.b_trainingPlaceholder)
        #드롭아웃. 학습할때만 적용된다. 
        ''' Whether to return the output in training mode (apply dropout) 
        or in inference mode (return the input untouched).'''

        return flow

    def __makeSoftmaxLayer__(self,n_onehotMethod,n_label,flow):
        #n_label은 분류 데이터의 갯수 
        
        #correct = tf.nn.in_top_k(flow,self.labelPlaceholder,1)

        n_in = int(self.li_shape[1])
        weight = tf.Variable(tf.truncated_normal([n_in,n_label],mean=0.0,stddev=0.1))
        bias = tf.Variable(tf.constant(0.1,shape=[n_label]))
        flow = tf.matmul(flow,weight) + bias  #분류를 위한 완전연결계층

        self.tfts_argmax = tf.argmax(flow,axis=1,name='predict')
        
        #소프트맥스 계층 이후로도 분류 결과는 달라지지 않을 것이기 때문에 이곳에서 argmax를 구한다.

        if n_onehotMethod == 0: #기본값. 원핫 인코딩이 아닌경우
            self.labelPlaceholder = tf.placeholder(tf.int64,shape=(None),name='label')
            self.isOneHot = False
            flow = tf.nn.sparse_softmax_cross_entropy_with_logits(logits = flow,labels = self.labelPlaceholder)
        elif n_onehotMethod == 1:#원핫 인코딩 벡터인 경우
            self.labelPlaceholder = tf.placeholder(tf.int64,shape=(None,n_label),name='label')
            self.isOneHot = True
            flow = tf.nn.softmax_cross_entropy_with_logits(logits = flow,labels = self.labelPlaceholder)
        
        
        correct = tf.equal(self.tfts_argmax,self.labelPlaceholder)
        self.accuracy = tf.reduce_mean(tf.cast(correct,tf.float32),name='accuracy')

        self.loss = tf.reduce_sum(flow,name='loss')
        flow = self.loss #오차는 옵티마이저에서 사용되어야 한다!

        return flow

    def __makeOptimizerLayer__(self,n_optimizer,f_momentum,f_learningRate,flow):

        print('__makeOptimizerLayer__ --> n_optimizer >>',n_optimizer)

        optimizer = None
        if n_optimizer == 0: #기본값 Adam
            optimizer = tf.train.AdamOptimizer(0.001)
        elif n_optimizer == 1:
            optimizer = tf.train.MomentumOptimizer(f_learningRate,momentum = f_momentum)
        elif n_optimizer == 2:
            optimizer = tf.train.MomentumOptimizer(f_learningRate,momentum = f_momentum,use_nesterov=True)
        elif n_optimizer == 3:
            optimizer = tf.train.RMSPropOptimizer(f_learningRate,momentum = 0.9,decay = 0.9,epsilon = 1e-10)
        elif n_optimizer == 4:#아담 옵티마이저의 경우 기본 학습률을 0.001로 설정해보자
            optimizer = tf.train.GradientDescentOptimizer(f_learningRate)
        else:
            print('n_optimizer --> ',n_optimizer)
            self.__error__('make optimize에서 에러')
        
        self.trainLayer = optimizer.minimize(flow)
        flow = self.trainLayer
        return flow
    
    def interpretPayload(self,filename):
        count = 0
        prevcode = 0
        while True:
            print('count -> ',count)
            count+=1
            code = struct.unpack('1i',self.remainPayload[:4])
            print('code -> ',code)
            code = int(code[0])
            self.remainPayload = self.remainPayload[4:]

            self.li_code.append(code)

            if code == 10099:#종료 코드라고 가정
                print('interpretPayload 함수 종료')
                break

            if code == 1001: #입력층인경우
                data = struct.unpack('1i',self.remainPayload[:4])
                self.remainPayload = self.remainPayload[4:]

                n_features = int(data[0])

                self.flow = self.__makeInputLayer__(n_features,self.tfplc_inputLayer,self.flow)
                self.li_shape = self.flow.get_shape()

            elif code == 1002:#합성곱 계층

                if len(self.li_shape) == 2:#이전 계층이 입력층이거나 완전연결계층이면 reshape과정을 거쳐야함
                    #즉 이 if문은 평탄화 되어 있는 벡터를 2차원의 이미지로 만들어주는 과정이다.
                    #원본 이미지가 3채널인것으로 가정한다. (BGR)
                    #self.n_in이 가로 또는 새로의 차원 정보를 가지고 있을것이다.
                    #code가 -1인 입력층인경우에서 n_in을 초기화 했을 것.
                    #지금은 완전연결계층에서 다시 합성곱으로 간 경우는 생각하지 않았다. 
                    n_features = int(self.li_shape[1]) #가령 가로4 세로 4 채널3의 이미지의 경우 이 값은 4*4*3일것
                    n_channel = 3
                    n_width = int((n_features/3)**0.5)#n_width의 값이 4가 되도록 하는 것 
                    
                    if n_width*n_width*3 != n_features:
                        self.__error__('합성곱계층 구성중 채널 오류!')

                    self.flow = tf.reshape(self.flow,shape=[-1,n_width,n_width,n_channel])

                self.li_shape = self.flow.get_shape()
                print('self.li_shape --> ',self.li_shape)###디버깅용
                data = struct.unpack('1i2f4i1f',self.remainPayload[:32])
                self.remainPayload = self.remainPayload[32:]

                n_initMethod, f_mean   , f_std     = int(data[0]), float(data[1]), float(data[2])
                n_filterSize, n_strides, n_padding = int(data[3]), int(data[4])  , int(data[5])
                n_neuron    , f_bias               = int(data[6]), float(data[7])

                self.flow = self.__makeConvLayer__(n_initMethod,f_mean,f_std,\
                n_filterSize,n_strides,n_padding,n_neuron,f_bias,self.flow)

                self.li_shape = self.flow.get_shape()
  
            elif code == 1003:#풀링계층
                
                data = struct.unpack('4i',self.remainPayload[:16])
                self.remainPayload = self.remainPayload[16:]

                n_filterSize,n_strides    = int(data[0]),int(data[1])
                n_padding,n_poolingMethod = int(data[2]),int(data[3])

                self.flow = self.__makePoolingLayer__(n_filterSize,n_strides,n_padding,n_poolingMethod,self.flow)
                self.li_shape = self.flow.get_shape()

            elif code == 1004:#완전연결계층
                if len(self.li_shape) == 4:#이전층이 합성곱 계층이면 평탄화 과정이 필요하다.
                    tu_prevshape = self.li_shape
                    r,c,chan = int(tu_prevshape[1]),int(tu_prevshape[2]),int(tu_prevshape[3])
                    self.flow = tf.reshape(self.flow,[-1,r*c*chan])#flatten
                    self.li_shape = self.flow.get_shape()

                data = struct.unpack('1i2f1i1f',self.remainPayload[:20])
                self.remainPayload = self.remainPayload[20:]

                n_initMethod,f_mean   = int(data[0]),float(data[1])
                f_std,n_neuron,f_bias = float(data[2]),int(data[3]),float(data[4])

                self.flow = self.__makeDenseLayer__(n_initMethod,f_mean,f_std,n_neuron,f_bias,self.flow)

                self.li_shape = self.flow.get_shape()

            elif code == 1005:#활성화 계층
                data = struct.unpack('1i',self.remainPayload[:4])
                self.remainPayload = self.remainPayload[4:]

                n_activationMethod = int(data[0])

                self.flow = self.__makeActivationLayer__(n_activationMethod,self.flow)

            elif code == 1006:#배치정규화계층
                data = struct.unpack('1i',self.remainPayload[:4])
                self.remainPayload = self.remainPayload[4:]

                f_momentumParm = float(data[0])

                self.flow = self.__makeBatchNormLayer__(f_momentumParm,self.flow)

            elif code == 1007:#드롭아웃계층
                data = struct.unpack('1f',self.remainPayload[:4])
                self.remainPayload = self.remainPayload[4:]

                f_dropoutRatio = float(data[0])

                self.flow = self.__makeDropoutLayer__(f_dropoutRatio,self.flow)

            elif code == 1008:#분류계층
                data = struct.unpack('2i',self.remainPayload[:8])
                self.remainPayload = self.remainPayload[8:]

                n_onehotMethod,n_label = int(data[0]),int(data[1])

                self.flow = self.__makeSoftmaxLayer__(n_onehotMethod,n_label,self.flow)

            elif code == 1009:#옵티마이저 계층
                print('Netclf --> generate --> remainPayload >> ',self.remainPayload[:12])
                data = struct.unpack('1i2f',self.remainPayload[:12])
                print('Netclf --> generate --> remainPayload >> ',data)
                self.remainPayload = self.remainPayload[12:]
                
                n_optimizer = int(data[0])
                f_momentum = float(data[1])
                f_learningRate = float(data[2])

                print('Netclf --> generate --> remainPayload >> optimizer >>'
                +str(n_optimizer)+' momentum >> '+str(f_momentum)+' lr >> '+str(f_learningRate))

                self.flow = self.__makeOptimizerLayer__(n_optimizer,f_momentum,f_learningRate,self.flow)

            prevcode = code

        #신경망을 구성하고 바로 디스크에 저장한다. 
        
        sess = tf.Session()
        sess.run(tf.global_variables_initializer())

        #filename은 서버/신경망/신경망이름/신경망이름 이다 
        #따라서 신경망이름으로 ckpt, txt 파일등을 만드는 것이다.

        saver = tf.train.Saver()
        saver.save(sess,filename+'.ckpt')

        self.__makeDict__()
        dSaver = dictSaver()
        dSaver.save_dict(self.dict_tsinfo,filename+'.txt')
        
        fileWriter = open(filename+PAYLOADBIN,'wb')
        fileWriter.write(self.byte_originalPayload)
        fileWriter.close()

        sess.close()

        tf.reset_default_graph() 

        print('interpretePayload--> 함수종료!')
        
        
    def __makeDict__(self):
        '''
        생성한 신경망의 주요 노드들을 딕셔너리로 만들어 반환한다. 
        '''
        if self.tfplc_inputLayer != None: #일단 지금은 입력층이 생성되었다면 신경망이 구성된 것으로 가정
            self.dict_tsinfo['input'] = self.tfplc_inputLayer.name
            self.dict_tsinfo['label'] = self.labelPlaceholder.name
            self.dict_tsinfo['loss'] = self.loss.name
            self.dict_tsinfo['accuracy'] = self.accuracy.name
            self.dict_tsinfo['predict'] = self.tfts_argmax.name
            self.dict_tsinfo['train'] = self.trainLayer.name
            self.dict_tsinfo['trainable'] = self.b_trainingPlaceholder.name

            #self.dict_tsinfo['prev_softmax'] = self.prev_softmax.name
            #self.dict_tsinfo['after_softmax'] = self.after_softmax.name
        else:
            self.__error__('아직 신경망이 구성되지 않았습니다. makeDict함수에서 에러')