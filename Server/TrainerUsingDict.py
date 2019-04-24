import numpy as np
import tensorflow as tf
from batchMaker import batchMaker
from dictSaver import dictSaver
import os

LABELINFO = 'LabelInfo.txt'
DATAINFO = 'DataInfo.txt'
ROOTDIR = './server'
SLASH = '/' #그냥 '/'를 쓰기 싫어서 만들어둔 상수입니다 ㅎ
NETDIR = 'net'
DATADIR = 'data'
RESURTDIR = 'result'
TRAINSET = 'trainset'          #이건 머지 
VECDIR = 'vecdir' 
class Trainer():
    '''
    트레이너 클래스는 구성된 신경망을 훈련시켜주는 클래스이다.
    생성자에서는 훈련 배치메이커, 시험 베치메이커, 파일이름을 인자로 받는다.
    store 함수는 현재 학습된 신경망을 파일로 저장한다.
    train 함수는 훈련데이터를 가지고 신경망을 학습한다.
    predict 함수는 신경망의 예측 결과를 반환한다. 
    accuracy_loss 함수는 정확도와 오차를 반환한다. 
    소멸자에서 세션을 close 한다 

    '''
    def __init__(self,trainBatchMaker:batchMaker,testBatchMaker:batchMaker,s_netname:str):
        '''
        훈련데이터를 가지고 있는 배치메이커, 테스트데이터를 가지고 있는 베치메이커
        신경망을 저장할때 사용한 filename 이 필요하다 
        이 filename을 가지고 그래프를 읽어오고 세션을 초기화하고
        딕셔너리를 초기화 할 것이다.
        '''
        #딕셔너리로 수정한 버전이다. 

        #tf.graph를 만들어서 사용해야 할 수도 있다. 지금은 기본 그래프에다가 모두다 추가하는것 같다. 
        self.test_batch = testBatchMaker
        self.train_batch = trainBatchMaker

        self.tfsess_sess = tf.Session()
        
        self.tfsaver = tf.train.import_meta_graph(s_netname+'.ckpt.meta')
        self.tfsaver.restore(self.tfsess_sess,s_netname+'.ckpt')
        
        self.dictsaver = dictSaver()
        self.dict_tsdata = self.dictsaver.import_dict(s_netname+'.txt')

        self.s_filename = s_netname

    def __del__(self):
        self.tfsess_sess.close()
        
    def store(self,newfilename = None):
        '''
        현재 trianer의 신경망과 딕셔너리를 디스크에 저장한다. 
        '''

        s_path = None
        if newfilename != None:
            s_path = newfilename
        else:
            s_path = self.s_filename
        saver = tf.train.Saver()
        saver.save(self.tfsess_sess,s_path+'.ckpt')
        self.dictsaver.save_dict(self.dict_tsdata,s_path+'.txt')


    def testfunc(self,n_batchsize):
        '''기능시험을 위해 만들었던 코드 입니다. 지워버려도 될듯'''
        data,label = self.test_batch.next_batch_using_numpy(n_batchsize)
        feed_dict = {self.dict_tsdata['input']:data,self.dict_tsdata['label']:label,self.dict_tsdata['trainable']:True}

        n3,n4 = self.tfsess_sess.run([self.dict_tsdata['predict'],self.dict_tsdata['accuracy']],feed_dict=feed_dict)
        print('predict \n',n3)
        print('accuracy \n',n4)
        pass

    def predict(self,n_batchSize,np2d_input = None,np2d_label = None):
        '''아직 입력크기가 1인경우는 다루지 않았다! 만약 들어온다면 reshape으로 2차원으로 만들어야 한다.'''
        #None이 아니면 사용자가 원하는 검증 데이터가 들어올 것으로 예상한다.
        feed_dict = None

        if np2d_input != None and np2d_label != None: #사용자가 원하는 데이터를 입력한경우
            feed_dict = {self.dict_tsdata['input']:np2d_input,self.dict_tsdata['label']:np2d_label,self.dict_tsdata['trainable']:False}
        else: #Trainer에게 입력으로 주었던 배치 메이커를 사용하는 경우
            data,label = self.test_batch.next_batch(n_batchSize)
            feed_dict = {self.dict_tsdata['input']:data,self.dict_tsdata['label']:label,self.dict_tsdata['trainable']:False}

        np1d_argmax = self.tfsess_sess.run(self.dict_tsdata['predict'],feed_dict = feed_dict)
        return np1d_argmax

    def train(self,n_batchSize,np2d_input = None,np2d_label = None):
        #None이 아니면 사용자가 원하는 검증 데이터가 들어올 것으로 예상한다.
        feed_dict = None

        if np2d_input != None and np2d_label != None: #사용자가 원하는 데이터를 입력한경우
            feed_dict = {self.dict_tsdata['input']:np2d_input,self.dict_tsdata['label']:np2d_label,self.dict_tsdata['trainable']:True}
        else: #Trainer에게 입력으로 주었던 배치 메이커를 사용하는 경우
            data,label = self.train_batch.next_batch_using_numpy(n_batchSize)
            feed_dict = {self.dict_tsdata['input']:data,self.dict_tsdata['label']:label,self.dict_tsdata['trainable']:True}

        self.tfsess_sess.run(self.dict_tsdata['train'],feed_dict = feed_dict)

    def accuracy_loss(self,n_batchsize,np2d_input = None,np2d_label = None):
        feed_dict = None

        if np2d_input != None and np2d_label != None: #사용자가 원하는 데이터를 입력한경우
            feed_dict = {self.dict_tsdata['input']:np2d_input,self.dict_tsdata['label']:np2d_label,self.dict_tsdata['trainable']:True}
        else: #Trainer에게 입력으로 주었던 배치 메이커를 사용하는 경우
            data,label = self.test_batch.next_batch_using_numpy(n_batchsize)
            feed_dict = {self.dict_tsdata['input']:data,self.dict_tsdata['label']:label,self.dict_tsdata['trainable']:True}

        nparr_accuracy,nparr_loss = self.tfsess_sess.run([self.dict_tsdata['accuracy'],self.dict_tsdata['loss']],feed_dict = feed_dict)        
        return nparr_accuracy,nparr_loss

    def makeModule(self):
        '''
        사용자의 디렉토리에 C:/clf/dftname디렉토리 안에 모든 ckpt파일과
        딕셔너리의 txt파일이 존재해야 합니다. 
        이는 클라이언트에서 설정해줘야 할것 같습니다. 
        '''
        '''아직 입력크기가 1인경우는 다루지 않았다! 만약 들어온다면 reshape으로 2차원으로 만들어야 한다."
        일단 지금은 trainer에 있는 코드를 그대로 가져다 넣어놓았다. 아마 동작하겠으나 필요하다면 나중에 수정해야 할 것이다.

        '''
        f = open(self.s_filename+'.py','w')
        end = '\n'
        s = ''
        s+="import tensorflow as tf"+end
        s+="import numpy as np"+end
        s+="import os"+end
        s+="from dictSaver import dictSaver"+end
        s+="class NetClassifier():"+end
        s+="    def __error__(self,s_error):"+end
        s+="        print(s_error)"+end
        s+="        exit(1)"+end
        s+="    def __init__(self):"+end
        s+="        self.s_ROOT = 'C:/clf'"+end
        s+="        self.s_NETNAME = 'dftname'"+end
        s+="        if not os.path.exists(self.s_ROOT):"+end
        s+="            self.__error__('C드라이브에 clf폴더가 없습니다!')"+end
        s+="        self.tfsess = tf.Session()"+end
        s+="        self.tfsaver = tf.train.import_meta_graph(self.s_ROOT+'/'+self.s_NETNAME+'/'+self.s_NETNAME+'.ckpt.meta')"+end
        s+="        self.tfsaver.restore(self.tfsess,self.s_ROOT+'/'+self.s_NETNAME+'/'+self.s_NETNAME+'.ckpt')"+end
        s+="        self.dictsaver = dictSaver()"+end
        s+="        self.dict_tsdata = self.dictsaver.import_dict(self.s_ROOT+'/'+self.s_NETNAME+'/'+self.s_NETNAME+'.txt')"+end
        s+="    def predict(self,n_batchSize,np2d_input,np2d_label):"+end
        s+="        feed_dict =  {self.dict_tsdata['input']:np2d_input,self.dict_tsdata['label']:np2d_label,self.dict_tsdata['trainable']:False}"+end
        s+="        np1d_argmax = self.tfsess.run(self.dict_tsdata['predict'],feed_dict = feed_dict)"+end
        s+="        return np1d_argmax"+end
        f.write(s)
        f.close()


class Trainer_Seperate():
    '''기존 트레이너 클래스에서 디스크로부터 학습 데이터를 받아올 수 있도록 한 클래스 입니다.
    딕셔너리로 훈련 데이터가 있는 디스크의 디렉토리 정보와 레이블 정보를 제공해야 합니다. 
    대규모 데이터셋을 한번에 메모리에 올릴수 없어서 디스크에서 읽어들이는 방식으로 구현한 것입니다. 
    트레이너 클래스는 구성된 신경망을 훈련시켜주는 클래스이다.
    생성자에서는 훈련 배치메이커, 시험 베치메이커, 파일이름을 인자로 받는다.
    store 함수는 현재 학습된 신경망을 파일로 저장한다.
    train 함수는 훈련데이터를 가지고 신경망을 학습한다.
    predict 함수는 신경망의 예측 결과를 반환한다. 
    accuracy_loss 함수는 정확도와 오차를 반환한다. 
    소멸자에서 세션을 close 한다 
    '''
    def __init__(self,dict_dataInfo:dict,dict_labelInfo:dict,s_netname:str):
        '''
        훈련데이터를 가지고 있는 배치메이커, 테스트데이터를 가지고 있는 베치메이커
        신경망을 저장할때 사용한 filename 이 필요하다 
        이 filename을 가지고 그래프를 읽어오고 세션을 초기화하고
        딕셔너리를 초기화 할 것이다.
        '''

        self.dict_csvFile = dict() #csv파일 리스트를 가지고 있는 딕셔너리
        self.n_csvLen = len(dict_dataInfo) #레이블의 갯수 
        self.li_csvFileLen = [0] * self.n_csvLen #전체 파일 갯수
        #가령 리스트의 0번에는 0번째 레이블 데이터의 csv 파일 갯수가 들어가 있다. 3개라면 3

        self.li_csvFileCur = [0] * self.n_csvLen #현재 파일 인덱스
        #지금사용한 csv파일의 인덱스가 들어가있다. 가령 현재 번째가 vec2.csv를 사용했다면 2
        #즉 이 두 리스트는 파일 갯수와 참조되고 있는 파일을 가르키는 포인터라고 할 수 있다. 
        for label,vecdirname in dict_dataInfo.items():
            filenames = os.listdir(vecdirname)
            print('디버깅용 Trainer_Seperate -> filenames >> ',filenames)
            self.dict_csvFile[label] = filenames #csv파일들의 이름
            self.li_csvFileLen[label] = len(filenames) 
        print('디버깅용 Trainer_Seperate -> self.dict_csvFile >> ',self.dict_csvFile)
        self.dict_dataInfo = dict_dataInfo
        self.dict_labelInfo = dict_labelInfo

        self.test_batch = None
        self.train_batch = None

        self.allocateBatchMaker()

        self.tfsess_sess = tf.Session()
        
        self.tfsaver = tf.train.import_meta_graph(s_netname+'.ckpt.meta')
        self.tfsaver.restore(self.tfsess_sess,s_netname+'.ckpt')
        
        self.dictsaver = dictSaver()
        self.dict_tsdata = self.dictsaver.import_dict(s_netname+'.txt')

        self.s_filename = s_netname

    def allocateBatchMaker(self):
        dict_csvFileInfo = dict()
        for label in range(self.n_csvLen):
            filenames = self.dict_csvFile[label]
            n_curCSVFileIdx = self.li_csvFileCur[label]
            dict_csvFileInfo[label] = filenames[n_curCSVFileIdx]
            self.li_csvFileCur[label] += 1
            if self.li_csvFileCur[label] == self.li_csvFileLen[label]: #한번씩 다 사용했다면
                self.li_csvFileCur[label] = 0  #다시 처음부터 사용
        print('Trainer_Seperate -> allocateBatchMaker >> self.dict_csvFileInfo ,',dict_csvFileInfo)
        np_alldata = batchMaker.STATIC_concatCSV_using_numpy_write_label_with_dict(dict_csvFileInfo)
        np_train,np_test = batchMaker.STATIC_setNParr_splitData_using_numpy(np_alldata)
        self.test_batch,self.train_batch = batchMaker(),batchMaker()
        self.train_batch.setNpArr_using_numpy(np_train)
        self.test_batch.setNpArr_using_numpy(np_test)

    def __del__(self):
        self.tfsess_sess.close()
        
    def store(self,newfilename = None):
        '''
        현재 trianer의 신경망과 딕셔너리를 디스크에 저장한다. 
        '''

        s_path = None
        if newfilename != None:
            s_path = newfilename
        else:
            s_path = self.s_filename
        saver = tf.train.Saver()
        saver.save(self.tfsess_sess,s_path+'.ckpt')
        self.dictsaver.save_dict(self.dict_tsdata,s_path+'.txt') #신경망의 주요 노드 저장
        self.dictsaver.save_dict(self.dict_labelInfo,s_path+LABELINFO) #레이블 정보 저장
        self.dictsaver.save_dict(self.dict_dataInfo,s_path+DATAINFO) #훈련 데이터 디렉토리 정보 저장
        
    def testfunc(self,n_batchsize):
        '''기능시험을 위해 만들었던 코드 입니다. 지워버려도 될듯'''
        data,label = self.test_batch.next_batch_using_numpy(n_batchsize)
        feed_dict = {self.dict_tsdata['input']:data,self.dict_tsdata['label']:label,self.dict_tsdata['trainable']:True}

        n3,n4 = self.tfsess_sess.run([self.dict_tsdata['predict'],self.dict_tsdata['accuracy']],feed_dict=feed_dict)
        print('predict \n',n3)
        print('accuracy \n',n4)
        pass

    def predict(self,n_batchSize,np2d_input = None,np2d_label = None):
        '''아직 입력크기가 1인경우는 다루지 않았다! 만약 들어온다면 reshape으로 2차원으로 만들어야 한다.'''
        #None이 아니면 사용자가 원하는 검증 데이터가 들어올 것으로 예상한다.
        feed_dict = None

        if np2d_input != None and np2d_label != None: #사용자가 원하는 데이터를 입력한경우
            feed_dict = {self.dict_tsdata['input']:np2d_input,self.dict_tsdata['label']:np2d_label,self.dict_tsdata['trainable']:False}
        else: #Trainer에게 입력으로 주었던 배치 메이커를 사용하는 경우
            data,label = self.test_batch.next_batch(n_batchSize)
            feed_dict = {self.dict_tsdata['input']:data,self.dict_tsdata['label']:label,self.dict_tsdata['trainable']:False}
        
        try:
            np1d_argmax = self.tfsess_sess.run(self.dict_tsdata['predict'],feed_dict = feed_dict)
            return np1d_argmax
        except tf.errors.ResourceExhaustedError as e:
            print('배치 크기를 수정하세요')

    def train(self,n_batchSize,np2d_input = None,np2d_label = None):
        #None이 아니면 사용자가 원하는 검증 데이터가 들어올 것으로 예상한다.
        if self.train_batch.n_reuseCnt > 2:
            self.allocateBatchMaker()

        feed_dict = None

        if np2d_input != None and np2d_label != None: #사용자가 원하는 데이터를 입력한경우
            feed_dict = {self.dict_tsdata['input']:np2d_input,self.dict_tsdata['label']:np2d_label,self.dict_tsdata['trainable']:True}
        else: #Trainer에게 입력으로 주었던 배치 메이커를 사용하는 경우
            data,label = self.train_batch.next_batch_using_numpy(n_batchSize)
            feed_dict = {self.dict_tsdata['input']:data,self.dict_tsdata['label']:label,self.dict_tsdata['trainable']:True}
        try:
            self.tfsess_sess.run(self.dict_tsdata['train'],feed_dict = feed_dict)
            return True
        except tf.errors.ResourceExhaustedError as e:
            print('배치크기를 수정하세요 ')
            return False

    def accuracy_loss(self,n_batchsize,np2d_input = None,np2d_label = None):
        feed_dict = None

        if np2d_input != None and np2d_label != None: #사용자가 원하는 데이터를 입력한경우
            feed_dict = {self.dict_tsdata['input']:np2d_input,self.dict_tsdata['label']:np2d_label,self.dict_tsdata['trainable']:True}
        else: #Trainer에게 입력으로 주었던 배치 메이커를 사용하는 경우
            data,label = self.test_batch.next_batch_using_numpy(n_batchsize)
            feed_dict = {self.dict_tsdata['input']:data,self.dict_tsdata['label']:label,self.dict_tsdata['trainable']:True}

        nparr_accuracy,nparr_loss = self.tfsess_sess.run([self.dict_tsdata['accuracy'],self.dict_tsdata['loss']],feed_dict = feed_dict)        
        return nparr_accuracy,nparr_loss

    def makeModule(self):
        '''
        사용자의 디렉토리에 C:/clf/dftname디렉토리 안에 모든 ckpt파일과
        딕셔너리의 txt파일이 존재해야 합니다. 
        이는 클라이언트에서 설정해줘야 할것 같습니다. 
        '''
        '''아직 입력크기가 1인경우는 다루지 않았다! 만약 들어온다면 reshape으로 2차원으로 만들어야 한다."
        일단 지금은 trainer에 있는 코드를 그대로 가져다 넣어놓았다. 아마 동작하겠으나 필요하다면 나중에 수정해야 할 것이다.

        '''
        f = open(self.s_filename+'.py','w')
        end = '\n'
        s = ''
        s+="import tensorflow as tf"+end
        s+="import numpy as np"+end
        s+="import os"+end
        s+="from dictSaver import dictSaver"+end
        s+="class NetClassifier():"+end
        s+="    def __error__(self,s_error):"+end
        s+="        print(s_error)"+end
        s+="        exit(1)"+end
        s+="    def __init__(self):"+end
        s+="        self.s_ROOT = 'C:/clf'"+end
        s+="        self.s_NETNAME = 'dftname'"+end
        s+="        if not os.path.exists(self.s_ROOT):"+end
        s+="            self.__error__('C드라이브에 clf폴더가 없습니다!')"+end
        s+="        self.tfsess = tf.Session()"+end
        s+="        self.tfsaver = tf.train.import_meta_graph(self.s_ROOT+'/'+self.s_NETNAME+'/'+self.s_NETNAME+'.ckpt.meta')"+end
        s+="        self.tfsaver.restore(self.tfsess,self.s_ROOT+'/'+self.s_NETNAME+'/'+self.s_NETNAME+'.ckpt')"+end
        s+="        self.dictsaver = dictSaver()"+end
        s+="        self.dict_tsdata = self.dictsaver.import_dict(self.s_ROOT+'/'+self.s_NETNAME+'/'+self.s_NETNAME+'.txt')"+end
        s+="    def predict(self,n_batchSize,np2d_input,np2d_label):"+end
        s+="        feed_dict =  {self.dict_tsdata['input']:np2d_input,self.dict_tsdata['label']:np2d_label,self.dict_tsdata['trainable']:False}"+end
        s+="        np1d_argmax = self.tfsess.run(self.dict_tsdata['predict'],feed_dict = feed_dict)"+end
        s+="        return np1d_argmax"+end
        f.write(s)
        f.close()
