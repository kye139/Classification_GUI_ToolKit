import numpy as np
import tensorflow as tf

class batchMaker():
    '''
    batchMaker는 여러개의 csv 파일을 다루는 정적 함수와 
    하나의 csv 파일 또는 numpy 배열로부터 학습과 검증을 위한 
    배치 데이터를 다루는 내부 함수로 이루어져 있다. 

    STATIC_concatCSV: csv파일들의 이름을 입력으로 받아서 합친다음 
    outfilename으로 저장하거나 numpy 배열을 반환하는 함수

    STATIC_makeCSV: 2차원 numpy 배열을 입력으로 받아서 
    csv 파일로 저장하는 함수이다. 

    STATIC_setCSV_splitData: csv파일을 입력으로 받아서 
    훈련 데이터와 검증 데이터로 나누는 함수
    
    STATIC_setNParr_splitData: numpy 배열을 입력으로 받아서
    훈련 데이터와 검증 데이터로 나누는 함수 

    내부 함수에는 setCSV와 setNparr이 있다.
    하나의 batchMaker는 훈련 데이터 또는 검증 데이터를 하나만
    컨트롤이 가능하다. 따라서 실제로는 2개의 batchMaker를 
    사용해야 할 것이다. 
    next_batch 함수를 사용해서 원하는 갯수만큼의
    데이터를 반환받을 수 있다. 
    
    '''

    @staticmethod
    def STATIC_concatCSV_using_numpy_write_label_with_dict(dict_labelInfo:dict):
        '''
        STATIC_concatCSV_using_numpy를 개량하여 레이블을 딕셔너리로 주는 함수입니다. 
        filenames는 csv파일들의 이름 입니다. 
        dict_labelInfo는 key는 레이블 val은 디렉토리 이름 입니다.
        가령 key는 0 val은 ROOTDIR/username/DATADIR/dog 입니다. 
        '''
        np2d_arr = None
        for label,filename in dict_labelInfo.items():
            np2d_tmp = np.genfromtxt(filename,delimiter=',',dtype=np.float)
            r,c = np2d_tmp.shape
            np2d_label = np.empty((r,1))
            np2d_label.fill(label)
            np2d_tmp = np.concatenate((np2d_tmp,np2d_label),axis=1)

            if label == 0: #맨 처음 파일인 경우
                np2d_arr = np2d_tmp
            else:        #처음 파일이 아닌 경우
                np2d_arr = np.concatenate((np2d_arr,np2d_tmp)) 

        return np2d_arr

    @staticmethod
    def STATIC_concatCSV(filenames,s_outfilename,b_returnCSV = True,b_writeLabel = True):
        '''csv파일들을 입력으로 받아서 합친다음 outfilename으로 저장
        b_returnCSV를 False를 주면 함수에서 numpy 배열을 반환한다 
        b_writeLabel에 True를 주면 함수에서 이름에 따라 숫자라벨을 붙여 준다.
        '''
        for idx, filename in enumerate(filenames):
            fileOpener = open(filename,'r+')
            lines = []
            np2d_arr = None
            while True:
                line = fileOpener.readline()
                if not line: break
                line = line.split(',')
                if '\n' in line:
                    line.remove('\n')
                for idx,val in enumerate(line):
                    try:
                        line[idx] = float(val)
                    except Exception as e:
                        print(e)
                        continue
                lines.append(line)
                np2d_tmp = np.array(lines)
                if b_writeLabel:
                    r,c = np2d_tmp.shape
                    np2d_label = np.empty((r,1))
                    np2d_label.fill(idx)
                    np2d_tmp = np.concatenate((np2d_tmp,np2d_label),axis=1)

            if idx == 0: #맨 처음 파일인 경우
                np2d_arr = np2d_tmp
            else:        #처음 파일이 아닌 경우
                np2d_arr = np.concatenate((np2d_arr,np2d_tmp)) 
                #이부분에러 애러가 날 수 있다. 아마 안날수도 

        if b_returnCSV: #이부분을 하나의 process로 만들어도 되겠다.
            batchMaker.STATIC_makeCSV(s_outfilename,np2d_arr)
        else:
            return np2d_arr

    @staticmethod
    def STATIC_concatCSV_using_numpy(filenames:list,s_outfilename:str,b_returnCSV = True,b_writeLabel = True):
        '''csv파일들을 입력으로 받아서 합친다음 outfilename으로 저장
        b_returnCSV를 False를 주면 함수에서 numpy 배열을 반환한다 
        b_writeLabel에 True를 주면 함수에서 이름에 따라 숫자라벨을 붙여 준다.
        '''
        np2d_arr = None
        for idx, filename in enumerate(filenames):
            np2d_tmp = np.genfromtxt(filename,delimiter=',',dtype=np.float)
            if b_writeLabel: #라벨링
                r,c = np2d_tmp.shape
                np2d_label = np.empty((r,1))
                np2d_label.fill(idx)
                np2d_tmp = np.concatenate((np2d_tmp,np2d_label),axis=1)

            if idx == 0: #맨 처음 파일인 경우
                np2d_arr = np2d_tmp
            else:        #처음 파일이 아닌 경우
                np2d_arr = np.concatenate((np2d_arr,np2d_tmp)) 
                #이부분에러 애러가 날 수 있다. 아마 안날수도 

        if b_returnCSV: #이부분을 하나의 process로 만들어도 되겠다.
            batchMaker.STATIC_makeCSV_using_numpy(s_outfilename,np2d_arr)
        else:
            return np2d_arr

    @staticmethod
    def STATIC_makeCSV_using_numpy(path,nparr2d_data:np.ndarray):
        np.savetxt(path,nparr2d_data,delimiter=',',fmt='%.8f')

    @staticmethod
    def STATIC_makeCSV(path,nparr2d_data):
        '''2차원 numpy 배열을 입력으로 받아서 csv파일로 저장하는 함수'''
        if nparr2d_data.ndim != 2:
            print('nparr2d_data가 2차원이 아니다!')
            pass
        r,c = nparr2d_data.shape
        #동작 확인이 된다면 멀티프로세싱이나 더 좋은 다른 알고리즘으로 바꾸어 보자 너무 느리다.
        buf = ''
        for r_idx in range(r):
            print('STATIC_makeCSV -> r_idx >>',r_idx)
            for c_idx in range(c):
                buf += (str(nparr2d_data[r_idx,c_idx])+',')
            buf+='\n'
        
        fileOpener = open(path,"w+")
        fileOpener.write(buf)
        fileOpener.close()
        print('STATIC_makeCSV -> fileOpner.close()')
     
    @staticmethod
    def STATIC_setNParr_splitData(np2d_data,f_ratio = 0.8,b_shuffle = True):
        # 2차원 numpy 읽어들여 훈련비율만큼으로 훈련데이터와 테스트데이터를 나누는 정적함수
        # 반환되는 값을 두 batchMaker로 받아서 사용하면 될 것이다. 
        # b_shuffle이 True이면 랜덤하게 훈련데이터와 검증데이터를 나눈다.
        # 그렇지 않으면 뒤에서부터 비율만큼 자른다.
        nparr2d = np2d_data
        n_datalen = len(nparr2d)
        idx = np.arange(0,n_datalen)        
        if b_shuffle:
            np.random.shuffle(idx)
        n_traindata = int(n_datalen*f_ratio)
        train_idx = idx[:n_traindata]
        test_idx = idx[n_traindata:]
        np_train,np_test = nparr2d[train_idx],nparr2d[test_idx]
        return np_train,np_test

    @staticmethod
    def STATIC_setNParr_splitData_using_numpy(np2d:np.ndarray,f_ratio = 0.8,b_shuffle = True):
        '''전체데이터를 입력하면 훈련데이터와 학습데이터로 나누어 반환하는 함수입니다. '''
        nparr2d = np2d
        n_datalen = nparr2d.shape[0]
        idx = np.arange(0,n_datalen)        
        if b_shuffle:
            np.random.shuffle(idx)
        n_traindata = int(n_datalen*f_ratio)
        train_idx = idx[:n_traindata]
        test_idx = idx[n_traindata:]
        np_train,np_test = nparr2d[train_idx],nparr2d[test_idx]
        return np_train,np_test
        
    @staticmethod
    def STATIC_setCSV_splitData_using_numpy(filename:str,f_ratio = 0.8,b_shuffle = True):

        nparr2d = np.genfromtxt(filename,delimiter=',',dtype=np.float)
        n_datalen = nparr2d.shape[0]
        idx = np.arange(0,n_datalen)        
        if b_shuffle:
            np.random.shuffle(idx)
        n_traindata = int(n_datalen*f_ratio)
        train_idx = idx[:n_traindata]
        test_idx = idx[n_traindata:]
        np_train,np_test = nparr2d[train_idx],nparr2d[test_idx]
        return np_train,np_test

    @staticmethod
    def STATIC_setCSV_splitData(filename,f_ratio = 0.8,b_shuffle = True):
        # csv파일을 읽어들여 훈련비율만큼으로 훈련데이터와 테스트데이터를 나누는 정적함수
        # 반환되는 값을 두 batchMaker로 받아서 사용하면 될 것이다. 
        # b_shuffle이 True이면 랜덤하게 훈련데이터와 검증데이터를 나눈다.
        # 그렇지 않으면 뒤에서부터 비율만큼 자른다.
        fileOpener = open(filename,'r+')
        lines = []
        while True:
            line = fileOpener.readline()
            if not line: break
            line = line.split(',')
            if '\n' in line:
                line.remove('\n')
            for idx,val in enumerate(line):
                try:
                    line[idx] = float(val)
                except Exception as e:
                    
                    continue
            lines.append(line)

        nparr2d = np.array(lines)
        n_datalen = len(nparr2d)
        idx = np.arange(0,n_datalen)        
        if b_shuffle:
            np.random.shuffle(idx)
        n_traindata = int(n_datalen*f_ratio)
        train_idx = idx[:n_traindata]
        test_idx = idx[n_traindata:]
        np_train,np_test = nparr2d[train_idx],nparr2d[test_idx]
        return np_train,np_test

    def __init__(self):
        self.fileOpener = None
        self.nparr2d = None
        self.npVec = None
        self.npLabel = None

        #tf를 사용하여 배치를 만들때 사용하는 코드들
        #for문을 사용하기 때문에 느려서 numpy를 사용한 코드를 쓰자
        self.que = None
        self.vec = None
        self.label = None
        self.op_enque = None
        self.mySess = None
        self.op_queSize = None

        #numpy만을 사용하여 배치를 만들때 사용하는 코드들
        self.b_using_numpy = False
        self.n_curidx = 0
        self.n_numOfData = 0
        self.np_randIdx = None

        self.n_reuseCnt = 0

    def __enqueAll__(self):
        '''훈련데이터와 레이블을 나누고 큐에 모두 추가'''
        self.npVec = self.nparr2d[:,:-1]
        self.npLabel = self.nparr2d[:,-1:]
        self.npLabel = self.npLabel.astype(np.int32)
        r,c = self.npVec.shape

        for i in range(r):
            self.mySess.run(self.op_enque,\
            feed_dict = {self.vec:self.npVec[i], self.label:self.npLabel[i]})

    def __splitVecLabel__(self):
        '''randomshuffledque를 생성하고 operation을 생성'''
        r,c = self.nparr2d.shape
        self.que = tf.RandomShuffleQueue(capacity = 100000,\
        min_after_dequeue = 0,dtypes=[tf.float32,tf.int32],shapes=[[c-1],[1]])
        self.op_queSize = self.que.size()
        self.vec = tf.placeholder(tf.float32,shape=(None))
        self.label = tf.placeholder(tf.int32,shape=(1))

        self.op_enque = self.que.enqueue((self.vec,self.label))

    def setNpArr_using_numpy(self,nparr:np.ndarray):
        #numpy 행렬을 입력으로 하여 배치메이커를 생성하는 것
        self.b_using_numpy = True

        self.nparr2d = nparr
        self.npVec = self.nparr2d[:,:-1]
        self.npLabel = self.nparr2d[:,-1:]
        r,c = self.nparr2d.shape
        self.n_numOfData = r

    def setCSV_using_numpy(self,filename:str):
        #기능시험완료!
        self.b_using_numpy = True

        self.nparr2d = np.genfromtxt(filename,delimiter=',',dtype=np.float)
        self.npVec = self.nparr2d[:,:-1]
        self.npLabel = self.nparr2d[:,-1:]
        r,c = self.nparr2d.shape
        self.n_numOfData = r

    def setBatchMethod(self,n_batchMethod:int):
        '''여러가지 배치 방법을 설정해보자 그러나 당장은 생각나는게 많지 않다.'''
        pass

    def next_batch_using_numpy(self,batchsize:int):
        #기능시험이 필요합니다!!
        '''numpy만 사용하여 배치를 만드는 방법 반복문을 돌지 않기 때문에 더 빠를 것이다. 
        이것을 사용하여 실제로 구현하도록 하자. 이전것들은 기존 코드를 남겨두기 위해서 그냥 둔 것'''
        if self.np_randIdx is None:
            self.np_randIdx = np.arange(0,self.n_numOfData)
            np.random.shuffle(self.np_randIdx)
        
        '''이곳에 여러가지 배치방법에 대해서 코드를 작성하면 될것 같다. 
        가령 모든 데이터 셋을 다 사용한 경우 다시 처음부터 데이터를 가져올지 
        아니면 다시 random하게 순서를 매겨서 데이터를 가져올지를 결정해야 할 것이다.
        일단은 그냥 순서대로 넣는 방법을 생각해 보자.
        123 451 234'''
        npCurVec,npCurLabel,np_curIdx = None,None,None

        '''여기서는 한번의 배치사이즈가 훈련데이터 전체를 여러번 돌아서 다 채워지는 상황은
        가정하지 않았다. 즉 배치사이즈가 30인데 전체 훈련 데이터가 5여서 5개를 6번 돌아서
        30을 채운후에 반환하게는 구현되어 있지 않다. 그것은 나중에 알고리즘을 생각해서 추가할 것이다.'''
        if self.n_curidx + batchsize > self.n_numOfData:
            self.n_reuseCnt += 1 

            np_curIdx = self.np_randIdx[self.n_curidx:self.n_numOfData]
            npCurVec,npCurLabel = self.npVec[np_curIdx],self.npLabel[np_curIdx]
            self.n_curidx = self.n_curidx + batchsize - self.n_numOfData

            np_curIdx = self.np_randIdx[0:self.n_curidx]
            npCurVec = np.concatenate((npCurVec,self.npVec[np_curIdx]))
            npCurLabel = np.concatenate((npCurLabel,self.npLabel[np_curIdx]))
            npCurLabel = npCurLabel.astype(np.int64)

            return npCurVec,npCurLabel.reshape(batchsize,)
            #return의 Label을 1-D 텐서로 만들자 즉 1차원 배열로 만들자!
        
        else:
            np_curIdx = self.np_randIdx[self.n_curidx:self.n_curidx+batchsize]
            self.n_curidx += batchsize
            npCurLabel = self.npLabel[np_curIdx].reshape(batchsize,)
            npCurLabel = npCurLabel.astype(np.int64)
            return self.npVec[np_curIdx],npCurLabel

    def setCSV(self,filename):
        '''
        csv파일 이름을 입력으로 받아서 2d numpy 배열을 만들고
        shuffled que에 레이블과 데이터를 짤라서 추가하는 함수
        데이터와 레이블을 짤라서 넣는 것에 주의!
        '''
        self.mySess = tf.Session()
        self.fileOpener = open(filename,'r+')
        lines = []
        while True:
            line = self.fileOpener.readline()
            if not line: break
            line = line.split(',')
            if '\n' in line:
                line.remove('\n')
            for idx,val in enumerate(line):
                try:
                    line[idx] = float(val)
                except Exception as e:
                    
                    continue
            lines.append(line)
        self.nparr2d = np.array(lines)

        self.__splitVecLabel__()
        self.__enqueAll__()

    def setNparr(self,nparr):
        '''
        2차원 nparray를 받는 함수
        setCSV함수와 동일
        '''
        self.mySess = tf.Session()
        if nparr.ndim != 2:
            print('nparr.ndim이 2가 아니다')
            pass

        self.nparr2d = nparr

        self.__splitVecLabel__()
        self.__enqueAll__()

    def next_batch(self,batchsize):
        if self.b_using_numpy == True:
            print('b_using_numpy 가 True 입니다! next_batch_using_numpy를 사용하세요!')
            pass
        '''배치사이즈 만큼 데이터를 반환하는 함수'''
        n_size = self.mySess.run(self.op_queSize)
        
        if n_size > batchsize:
            op_deque_many = self.que.dequeue_many(batchsize)
            vec,label = self.mySess.run(op_deque_many)
            
            return vec,label.reshape(batchsize,)
        else:
            self.__enqueAll__()
            op_deque_many = self.que.dequeue_many(batchsize)
            vec,label = self.mySess.run(op_deque_many)
            return vec,label.reshape(batchsize,)
        
        

class concatter():
    '''
    concatter의 기능은 대부분 batchMaker의 정적함수로 옮겨갔다.
    파일이름들을 입력을 받아서 하나의 numpy 배열을 만드는 클래스
    '''
    MINIMUM = 0
    MAXIMUM = 1
    def __init__(self,filenames):
        self.nparr2d_data = None
        self.trainData = None
        self.testData = None
        self.lines = []
        self.fileHandlers = list()
        for fname in filenames:
            handler = open(fname,'r')
            self.fileHandlers.append(handler)

        self.n_fileHandlersNum = len(self.fileHandlers)
        self.__make2dData__()
    
            

    def __make2dData__(self):
        #결과 데이터가 1,2,3,1,2,3,1,2,3 ... 형식으로 나열됨 (1,2,3은 각각 파일)
        b_out = False
        while True:

            for i in range(self.n_fileHandlersNum):
                #지금은 파일 갯수마다 한번씩 돌면서 데이터를 append한다.
                #비효율적일것 같음. 
                #속도 향상을 위해서는 np.array를 사용할 수 있을것 같다. 
                #np.concatenate함수를 사용해보자.

                line = self.fileHandlers[i].readline()
                if not line:
                    #모든 파일중 하나라도 데이터가 먼저 떨어진다면 종료한다.
                    #즉 가장 적은 데이터셋을 가진 파일을 기준으로 한다.
                    b_out = True
                    break
                line = line.split(',')
                if '\n' in line:
                    line.remove('\n')
                line = [float(i) for i in line]
                self.lines.append(line)
            if b_out: break
        self.nparr2d_data = np.array(self.lines,dtype=np.float32)
    
    def __make2dDataFileByFile__(self):
        #결과 데이터가 1,1,1,2,2,3,3,3,3, ... 형식으로 나열됨
        
        for handler in self.fileHandlers:

            while True:
                line = handler.readline()
                if not line:
                    break
                self.lines.append(line)
        self.nparr2d_data = np.array(self.lines,dtype = np.float32)

    def getAll2Darr(self):
        return self.nparr2d_data

    def getTrain2Darr(self):
        pass

    def getTest2Darr(self):
        pass



    def makeCSV(self,path):
        r,c = self.nparr2d_data.shape
        buf = ''
        for r_idx in range(r):
            for c_idx in range(c):
                buf += (str(self.nparr2d_data[r_idx,c_idx])+',')
            buf+='\n'
        fileOpener = open(path,"w+")
        fileOpener.write(buf)
        fileOpener.close()


def concattertest_main():
    filenames = ['./work/np2d_ori_sorted_label1_100.csv','./work/np2d_ori_sorted_label0_100.csv']
    myconcatter = concatter(filenames)
    myconcatter.makeCSV('./work/new_label01all_data100_3.csv')



def batchtest_main():
    batch = batchMaker()
    batch.setCSV('./work/label01all_data300_4096.csv')
    while True:
        vec,label = batch.next_batch(20)
        print('vec : ',vec)
        print('label : ',label)
        input('continue?')

def batch_split_main():
    np2d_train,np2d_test = batchMaker.STATIC_setCSV_splitData('./work/label01all_data300_4096.csv')
    train_batch = batchMaker()
    train_batch.setNparr(np2d_train)
    test_batch = batchMaker()
    test_batch.setNparr(np2d_test)

    while True:
        print('train ------')
        vec,label = train_batch.next_batch(20)
        print('vec : ',vec)
        print('label : ',label)
        print('\n')
        vec,label = test_batch.next_batch(20)
        print('vec : ',vec)
        print('label : ',label)     
        input('continue?')

def batch_split_main_using_numpy():
    np2d_train,np2d_test = batchMaker.STATIC_setCSV_splitData_using_numpy('./server/david/data/cat/vec.csv')
    print(np2d_train.shape)
    print(np2d_test.shape)
    input(':')
    train,test = batchMaker(),batchMaker()
    train.setNpArr_using_numpy(np2d_train)
    test.setNpArr_using_numpy(np2d_test)
    while True:
        print('train ------')
        vec,label = train.next_batch_using_numpy(20)
        print('vec : ',vec)
        print('label : ',label)
        print('\n')
        vec,label = test.next_batch_using_numpy(20)
        print('vec : ',vec)
        print('label : ',label)     
        input('continue?')


def batch_using_numpy_main():
    bm = batchMaker()
    bm.setCSV_using_numpy('./server/david/data/cat/vec.csv')
    while True:
        v,l = bm.next_batch_using_numpy(104)
        print(v.shape)
        print(v)
        print(l.shape)
        print(l)
        input('continue?')

def main():
    
    pass 

if __name__ == "__main__":
    batch_split_main_using_numpy()

