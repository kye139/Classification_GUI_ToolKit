import time
import threading
from multiprocessing import Process
import struct
import os
import zipfile
import numpy as np
import cv2
import shutil
import errno
import tensorflow as tf
from socket import *
from time import ctime
from NetClassfierGenerator import NetClassfierGenerator
from batchMaker import batchMaker,concatter
from TrainerUsingDict import Trainer,Trainer_Seperate
from dictSaver import dictSaver
from ModuleMaker import ModuleMaker

'''
회원가입 생각해보고
산출물 디렉토리: 시간단위로 저장해보기

'''


HOST = ''
PORT = 10000
BUFSIZE = 1024*1024
ADDR = (HOST,PORT)
ROOTDIR = './server'
SLASH = '/' #그냥 '/'를 쓰기 싫어서 만들어둔 상수입니다 ㅎ
NETDIR = 'net'
DATADIR = 'data'
RESURTDIR = 'result'
TRAINSET = 'trainset'          #이건 머지 
VECDIR = 'vecdir'              #서버/신경망/사용자신경망/vecdir <- vec0 vec1 이 저정

#NOTE: 아래 파일들은 net/신경망이름/신경망이름+ @@@@ 에 들어간다. 
PAYLOADBIN = 'PayloadBin.txt'  #신경망 구성시 전달된 페이로드를 저장
LABELINFO = 'LabelInfo.txt'    #훈련을 시작했을때 레이블연결된 정보 저장
DATAINFO = 'DataInfo.txt'      #훈련 데이터의 디렉토리 정보 저장 

OPCODE_CREATECLF = 1
OPCODE_STOREDATA = 2
OPCODE_PRETRAINING = 3
OPCODE_TRAINSET = 4
OPCODE_STORECURNET = 5
OPCODE_REQUESTMODULE = 6

OPCODE_CONNECTION_TEST = 10
OPCODE_CONNECTION = 11
OPCODE_SIGNUP = 12
OPCODE_LISTDATADIR = 13
OPCODE_LISTNETDIR = 14    ###
OPCODE_LISTRESULTDIR = 15
OPCODE_LISTNETDIRNODATA = 16
OPCODE_SENDIMGZIP = 17  #압축파일 전송 
OPCODE_SEND_TRAIN_CUR_PROCESS = 18  #현재 훈련 과정을 전송

OPCODE_ERROR = 100
OPCODE_HERATBEAT = 110
OPCODE_ERROR_GPU_RESOURCE_EXHAUSTED = 101

OPCODE_EXIT = 1000


class TCPserver(threading.Thread):

    @staticmethod
    def STATIC_img2vec_seperate(s_dir:str):
        '''영상데이터를 분할하여 디스크에 저장하는 함수입니다. 
        vec0.csv부터 vec#.csv 까지 저장됩니다. '''
        '''기능시험이 필요합니다. '''
        filenames = os.listdir(s_dir) #s_dir은 data/bird/ 과 같은 형식 예상
        np2d_imgvec = None
        n_idx,n_vecidx = 0,0 #0부터 100까지 반복하는 변수, csv파일의 번호를 나타내는 변수
        if not os.path.exists(s_dir+'/'+VECDIR): #VECDIR은 bird 안에 들어있는 vec0 vec1 이 들어있는 디렉토리
            os.mkdir(s_dir+'/'+VECDIR)
        for s_imgname in filenames:
            if n_idx % 100 == 0 and n_idx is not 0:
                batchMaker.STATIC_makeCSV_using_numpy(s_dir+'/'+VECDIR+'/vec'+str(n_vecidx)+'.csv',np2d_imgvec)
                n_vecidx += 1
                n_idx = 0
            img = cv2.imread(s_dir+'/'+s_imgname)
            ##### NOTE
            img = img.astype(np.float32)
            img /= 255.0
            npravel = np.ravel(img,order='C')
            if n_idx == 0:
                np2d_imgvec = np.reshape(npravel,(1,-1)) 
            else:
                npravel = np.reshape(npravel,(1,-1))
                np2d_imgvec =  np.concatenate((np2d_imgvec,npravel))
            n_idx += 1

    @staticmethod
    def STATIC_img2vec(s_dir:str):
        #s_dir은 이미지들이 들어있는 디렉토리의 위치가 필요하다.
        #기능 시험 완료

        #디렉토리 내의 모든 jpg png 파일을 읽어들여서 벡터로 저장한다. 
        #일단은 정적함수로 만드는데 혹시 오류가난다면 전역함수로 생성하자
        #이건 하나의 프로세스로 해서 동작되게 한다.
        filenames = os.listdir(s_dir)
        li_jpgpng = [f for f in filenames if f[-4:] == '.jpg' or f[-4:] == '.png']
        #NOTE: 이거 제거 하자 
        print('STATIC_img2vec -> li_jpgpng >>',li_jpgpng)

        #이제 여기서 프로세스를 하나 만들어서 모든 이미지 파일을 백터로 바꾸고 csv로 저장하는 코드를 추가하자.
        #모든 이미지는 동일한 크기라고 가정한다.
        np2d_imgvec,row,col,cn = None,None,None,3

        #동작 확인이 되면 4개로 잘라서 멀티 프로세싱으로 concat 한다음에 마지막에 이어 붙여도 될것 같다.
        #NOTE: 이부분이 느려터졌다 수정해야한다.
        for idx,s_imgname in enumerate(li_jpgpng):
            if idx%50 == 0:
                print('STATIC_img2vec -> s_imgname >>',s_imgname)
            img = cv2.imread(s_dir+'/'+s_imgname)
            ##### NOTE
            img = img.astype(np.float32)
            img /= 255.0
            npravel = np.ravel(img,order='C')
            if idx == 0:
                np2d_imgvec = np.reshape(npravel,(1,-1)) 
            else:
                npravel = np.reshape(npravel,(1,-1))
                np2d_imgvec =  np.concatenate((np2d_imgvec,npravel))

        batchMaker.STATIC_makeCSV_using_numpy(s_dir+SLASH+'vec.csv',np2d_imgvec)
            
    def __init__(self,sock_client,mutex,dict_userConnectionInfo:dict):
        threading.Thread.__init__(self)
        self.tcpClientSocket = sock_client
        self.filename = None #사용안하는 변수인듯 
        self.s_username = None
        self.s_curTrainingCSV = None
        self.curTrainer = None
        self.mutexlock = mutex
        self.dict_ConnectionInfo = dict_userConnectionInfo
        
    def recvFromClnt(self):
        '''클라이언트에게서 수신한후에 명령코드(4),페이로드길이(4),페이로드를 잘라서 반환하는 함수'''
        data = bytes(0)
        n_opAndPaySize = 8
        try:
            while True:
                tmp = self.tcpClientSocket.recv(n_opAndPaySize - len(data))
                data += tmp
                if len(data) == n_opAndPaySize: break
                time.sleep(1) #busy waiting을 간단히 막는 방법 적당히 동작함
            print('data ',data) #디버깅용
        except Exception as e:
            print('TCPserver --> recvFromClnt >> exception info >> ',e)
            exit(0)
        '''
        while True:
            try:
                data = self.tcpClientSocket.recv(8) #명령코드와 페이로드 길이를 받는다. 
                print('data',data)#디버깅
                print('datalen ',len(data))
                break
            except error, e:
                err = e.args[0]

                continue
                '''
        data = struct.unpack('2i',data[:8])
        print('recvFromClnt --> data >> ',data)#디버깅용
        n_opcode,n_size = int(data[0]),int(data[1])
        print('recvFromClnt --> n_size >> ',n_size) #페이로드의 크기가 n_size일것이다. 
        n_remain = n_size #남은 사이즈는 n_remain
        byte_dest = bytes(0) #목적지 
        while n_remain >0: #남은 바이트가 없을때까지 반복
            byte_buf = bytes(0) #바이트 초기화
            if n_remain <BUFSIZE: #남은 크기가 버퍼보다 작은 경우 
                byte_recv = bytes(0) 
                n_remainbuf = n_remain #이번 호출에서 읽어들여야 하는 전체 크기
                while len(byte_buf) <n_remain:
                    byte_recv = self.tcpClientSocket.recv(n_remainbuf)
                    byte_buf += byte_recv
                    n_remainbuf -= len(byte_recv)
                n_remain -= n_remain
            else:                 #남은 크기가 버퍼보다 큰 경우
                byte_recv = bytes(0) #한번의 recv함수에서 읽어들이는 크기
                n_remainbuf = BUFSIZE #한번에 BUFSIZE만큼을 꽉 채워서 읽어 들여야한다.
                while len(byte_buf) <BUFSIZE: #BUFSIZE만큼 읽어들이지 못한동안 반복
                    byte_recv = self.tcpClientSocket.recv(n_remainbuf) #
                    byte_buf += byte_recv #읽어들인 바이트 저장
                    n_remainbuf -= len(byte_recv) #BUFSIZE만큼에서 읽어들인 크기 만큼 뺌
                n_remain -= BUFSIZE

            byte_dest += byte_buf #BUFSIZE만큼 저장 또는 마지막인경우 읽어들인 바이트 만큼을 저장한다. 
        return n_opcode,n_size,byte_dest
        #byte_dest가 0일경우에는 어떻게 되는 것일까... 에러가 날려나 흠..

    def sendToClnt(self,n_opcode:int,n_size:int,byte_payload:bytes):
        print('TCPsrver --> sendToClnt >> n_opcode >> ',n_opcode)
        print('TCPsrver --> sendToClnt >> n_size >> ',n_size)
        byte_dest = bytes(0)
        byte_opcode = struct.pack('i',n_opcode)
        byte_size = struct.pack('i',n_size)
        byte_dest += byte_opcode
        byte_dest += byte_size
        byte_dest += byte_payload
        n_remain = len(byte_dest)
        while n_remain >0:
            byte_buf = None
            if n_remain < BUFSIZE:
                byte_buf = byte_dest[:n_remain]
                byte_dest = byte_dest[n_remain:] 
                n_remain -= n_remain
            else: 
                byte_buf = byte_dest[:BUFSIZE]
                byte_dest = byte_dest[BUFSIZE:]
                n_remain -= BUFSIZE
            self.tcpClientSocket.send(byte_buf)

    def recvZipFile(self,s_destfile = './joljak/default.zip'): 
        ##NOTE 이코드는 더이상 사용되지 않지만 원본유지를 위해 남겨둔다. 나중에 참고용 
        data = self.tcpClientSocket.recv(4) #압축파일의 길이를 먼저 읽어들인다.
        data = struct.unpack('i',data) #int형으로 바꾼다
        n_size = int(data[0])
        print('n_size >> ',n_size) #원본 크기는 n_size
        n_remain = n_size #남은 사이즈는 n_remain
        byte_dest = bytes(0) #목적지 
        while n_remain >0: #남은 바이트가 없을때까지 반복
            byte_buf = bytes(0) #바이트 초기화
            if n_remain <BUFSIZE: #남은 크기가 버퍼보다 작은 경우 
                byte_recv = bytes(0) 
                n_remainbuf = n_remain #이번 호출에서 읽어들여야 하는 전체 크기
                while len(byte_buf) <n_remain: 
                    byte_recv = self.tcpClientSocket.recv(n_remainbuf)
                    byte_buf += byte_recv
                    n_remainbuf -= len(byte_recv)
                n_remain -= n_remain
            else:                 #남은 크기가 버퍼보다 큰 경우
                byte_recv = bytes(0) #한번의 recv함수에서 읽어들이는 크기
                n_remainbuf = BUFSIZE #한번에 BUFSIZE만큼을 꽉 채워서 읽어 들여야한다.
                while len(byte_buf) <BUFSIZE: #BUFSIZE만큼 읽어들이지 못한동안 반복
                    byte_recv = self.tcpClientSocket.recv(n_remainbuf) #
                    byte_buf += byte_recv #읽어들인 바이트 저장
                    n_remainbuf -= len(byte_recv) #BUFSIZE만큼에서 읽어들인 크기 만큼 뺌
                n_remain -= BUFSIZE
            print('n_remain >> ',n_remain)
            byte_dest += byte_buf #BUFSIZE만큼 저장 또는 마지막인경우 읽어들인 바이트 만큼을 저장한다. 
        writer = open(s_destfile,'wb')
        writer.write(byte_dest)
        writer.close()

    def sendZipFile(self,filename):
        ##이코드는 더이상 사용되지 않는다. 원본 유지를 위해 남겨두고 나중에 참고용으로 사용한다.
        mystat = os.stat(filename)
        n_filesize = mystat.st_size
        n_remain = n_filesize
        opener = open(filename,'rb')
        byte_filesize = struct.pack('i',n_filesize)
        self.tcpClientSocket.send(byte_filesize)
        while n_remain >0:
            byte_buf = None
            if n_remain < BUFSIZE:
                byte_buf = opener.read(n_remain)
                n_remain -= n_remain
            else: 
                byte_buf = opener.read(BUFSIZE)
                n_remain -= BUFSIZE
            self.tcpClientSocket.send(byte_buf)
        opener.close()

    def dirSetting(self):
        '''필요한 디렉토리를 만드는 함수입니다. 
        username으로 디렉토리를 하나 만들고 data폴더와 net 폴더를 만듭니다'''
        #정상작동하는지 파악 되어야 합니다.
        if not os.path.exists(ROOTDIR+'/'+self.s_username):
            os.mkdir(ROOTDIR+'/'+self.s_username)
        if not os.path.exists(ROOTDIR+'/'+self.s_username+'/'+NETDIR):
            os.mkdir(ROOTDIR+'/'+self.s_username+'/'+NETDIR)
        if not os.path.exists(ROOTDIR+'/'+self.s_username+'/'+DATADIR):
            os.mkdir(ROOTDIR+'/'+self.s_username+'/'+DATADIR)
            os.mkdir(ROOTDIR+'/'+self.s_username+'/'+DATADIR+'/'+TRAINSET)
        if not os.path.exists(ROOTDIR+'/'+self.s_username+'/'+RESURTDIR):
            os.mkdir(ROOTDIR+'/'+self.s_username+'/'+RESURTDIR)
                
    def nameSetting(self,byte_payload:bytes):
        '''
        사용자의 이름을 읽어들이는 함수입니다. 
        '''
        #정상작동 확인!
        n_len = struct.unpack('i',byte_payload[:4])
        n_len = int(n_len[0])
        print('nameSetting -> n_len >> ',n_len)
        s_name = struct.unpack(str(n_len)+'c',byte_payload[4:])
        byte_dest = bytes()
        for b in s_name:
            byte_dest += b
        
        self.s_username = byte_dest.decode()
        print('nameSetting -> self.s_username >> ',self.s_username)

        ### 328 추가!
        self.mutexlock.acquire()
        self.dict_ConnectionInfo[self.s_username] = True
        print('TCPserver --> self.dict_ConnectionInfo >> ',self.dict_ConnectionInfo)
        self.mutexlock.release()
    
    def storeData(self,byte_payload:bytes):
        '''
        이미지 압축파일을 저장하는 함수 입니다. 
        레이블이름을 먼저 파악하고 그 이름으로 디렉토리를 생성하고
        파일을 저장합니다. 
        '''
        #정상작동 확인
        n_len = struct.unpack('i',byte_payload[:4])
        n_len = n_len[0]
        byte_payload = byte_payload[4:]
        s_name = struct.unpack(str(n_len)+'c',byte_payload[:n_len]) #레이블의 이름이다.
        byte_payload = byte_payload[n_len:]
        byte_dest = bytes(0)
        for b in s_name[:]:  #'\0'을 제거 하기 위해서 -1: 맨 뒤에서 한칸 전으로 설정
            byte_dest += b
        s_name = byte_dest.decode()
        
        if not os.path.exists(ROOTDIR+SLASH+self.s_username+SLASH+DATADIR+SLASH+s_name):
            os.mkdir(ROOTDIR+SLASH+self.s_username+SLASH+DATADIR+SLASH+s_name)
        f = open(ROOTDIR+SLASH+self.s_username+SLASH+DATADIR+SLASH+s_name+'.zip','wb')
        f.write(byte_payload)  #여기에는 모두 압축파일이 있을것으로 가정한다.
        f.close()
        
        zip = zipfile.ZipFile(ROOTDIR+SLASH+self.s_username+SLASH+DATADIR+SLASH+s_name+'.zip')
        zip.extractall(ROOTDIR+SLASH+self.s_username+SLASH+DATADIR+SLASH+s_name)
        zip.close()

        #proc = Process(target=TCPserver.STATIC_img2vec,args=(ROOTDIR+'/'+self.s_username+'/'+DATADIR+'/'+s_name,))
        proc = Process(target=TCPserver.STATIC_img2vec_seperate,args=(ROOTDIR+'/'+self.s_username+'/'+DATADIR+'/'+s_name,))
        #주석이 된 부분은 대규모 데이터셋에 대해서 처리하는 함수이다. 

        #args뒤에 ,를 붙이는 이유는 반복 가능한 객체를 만들기 위해서인것 같다. 그냥 (int) 이렇게 전달되면
        #반복가능한 객체가 아니기 때문에 에러가 나는것 같다. 
        proc.start()
        #여기서 프로세스를 실행시키면 프로세스가 detach된다.

    def prepareTraining_seperate(self,byte_payload:bytes):
        '''
        prepareTraining에서 디스크로부터 분할하여 데이터를 입력받도록 구현한 함수입니다. 
        기능시험이 필요합니다. 
        프로토콜은 신경망이름길이+신경망이름+데이터이름길이+데이터이름+데이터이름길이+데이터이름+마지막길이로-1
        이 함수가 호출되고 나면 신경망은 데이터에 종속됩니다. 
        '''
        
        '''
        li_label은 사용자가 사용하고자 하는 모든 훈련 데이터의 이름을 말한다.
        즉 dog, cat, bird 등의 char 배열 값을 의미한다.
        '''
        #기능 시험을 해보아야 한다.

        n_netCharSize = struct.unpack('i',byte_payload[:4])[0]
        byte_payload = byte_payload[4:]

        s_netname = struct.unpack(str(n_netCharSize)+'c',byte_payload[:n_netCharSize])
        byte_payload = byte_payload[n_netCharSize:]
        byte_dest = bytes(0)  # s_netname = b''.join(s_netname).decode()
        for b in s_netname[:]: 
            byte_dest += b
        s_netname = byte_dest.decode()  #사용할 신경망의 이름

        s_curNetPath = ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_netname+'/'+s_netname
        #서버/유저/신경망/사용자가사용하는신경망/사용자가사용하는신경망 
        
        if os.path.exists(s_curNetPath+LABELINFO): #이어서 학습하는 것이라면
            print('TCPserver --> prepareTrain_sep >> 이전에 학습된 정보가 있습니다 이어서 학습합니다. ')
            dict_saver = dictSaver()
            dict_dataInfo = dict_saver.import_dict(s_curNetPath + DATAINFO)
            dict_labelInfo = dict_saver.import_dict(s_curNetPath + LABELINFO)
            self.curTrainer = Trainer_Seperate(dict_dataInfo,dict_labelInfo,s_curNetPath)

        else: #처음 학습하는 것이라면 
            print('TCPserver --> prepareTrain_sep >> 새롭게 학습합니다. ')
            n_NumOfLabel = struct.unpack('i',byte_payload[:4])[0]
            byte_payload = byte_payload[4:]
            dict_dataInfo = dict()
            dict_labelInfo = dict()
            n_label = 0
            for _ in range(n_NumOfLabel): #레이블 갯수만큼 반복하도록 수정 
                n_charsize = struct.unpack('i',byte_payload[:4])[0]
                byte_payload = byte_payload[4:] #훈련데이터 이름의 길이를 받아온다.
                print('prepareTraining -> n_charsize[0] >> ',n_charsize)
                
                s_label = struct.unpack(str(n_charsize)+'c',byte_payload[:n_charsize])
                byte_payload = byte_payload[n_charsize:]

                byte_dest = bytes(0)
                for b in s_label[:]:
                    byte_dest += b
                s_label = byte_dest.decode() #s_label = b''.join(s_label).decode()
                s_vecdir = ROOTDIR+'/'+self.s_username+'/'+DATADIR+'/'+s_label+'/vecdir'
                dict_dataInfo[n_label] = s_vecdir
                dict_labelInfo[n_label] = s_label
                n_label += 1
                print('prepareTrainig_Seperate -> s_label >> ',s_label)
            print('prepareTrainig_Seperate -> dict_dataInfo >> ',dict_dataInfo)
            print('prepareTraining_Seperate --> dict_labelInfo >> ',dict_labelInfo)
            self.curTrainer = Trainer_Seperate(dict_dataInfo,dict_labelInfo,s_curNetPath)

        print('preparing Training --> 함수종료!')

    def prepareTraining(self,byte_payload:bytes):
        '''
        이전 코드 입니다. 기능이 몇개 없으니 사용하면 안됩니다. 
        '''
        '''
        li_label은 사용자가 사용하고자 하는 모든 훈련 데이터의 이름을 말한다.
        즉 dog, cat, bird 등의 char 배열 값을 의미한다.
        NOTE: 레이블의 이름을 10바이트로 제한할지 아니면 가변으로 할지를 결정해야 한다. 
        사용자가 원하는 훈련데이터들을 모아서 하나의 csv 파일로 만드는 함수
        cat dog bird 의 레이블을 사용한다면 cdb 라는 폴더를 생성하여 거기에 vec.csv를 생성
        신경망의 이름도 payload로 받아온다.
        그러면 서버내 curTrainer에 새로운 Trainer를 할당한다.
        '''
        #기능 시험을 해보아야 한다.

        n_netCharSize = struct.unpack('i',byte_payload[:4])
        byte_payload = byte_payload[4:]
        n_netCharSize = n_netCharSize[0]
        s_netname = struct.unpack(str(n_netCharSize)+'c',byte_payload[:n_netCharSize])
        byte_payload = byte_payload[n_netCharSize:]
        byte_dest = bytes(0)
        for b in s_netname[:]: 
            byte_dest += b
        s_netname = byte_dest.decode()

        li_label = []
        while True:
            n_charsize = struct.unpack('i',byte_payload[:4])
            byte_payload = byte_payload[4:] #훈련데이터 이름의 길이를 받아온다.
            n_charsize = n_charsize[0]
            print('prepareTraining -> n_charsize[0] >> ',n_charsize)
            if n_charsize == -1: #-1일 경우 종료
                break  
            s_label = struct.unpack(str(n_charsize)+'c',byte_payload[:n_charsize])
            byte_payload = byte_payload[n_charsize:]

            byte_dest = bytes(0)
            for b in s_label[:]:
                byte_dest += b
            s_label = byte_dest.decode()
            li_label.append(s_label)
            print('prepareTrainig -> s_label >> ',s_label)

        filenames = []
        s_outdir = ''
        for dirname in li_label:
            print('prepareTraining -> dirname >> ',dirname)
            s_outdir += dirname[0]
            s = ROOTDIR+'/'+self.s_username+'/'+DATADIR+'/'+dirname+'/'+'vec.csv'
            filenames.append(s)

        s_fulloutdir = ROOTDIR+'/'+self.s_username+'/'+DATADIR+'/'+s_outdir
        print('prepareTraining -> s_fulloutdir >> ',s_fulloutdir)
        if not os.path.exists(s_fulloutdir):#디렉터리가 존재한다면 삭제
            #shutil.rmtree(s_fulloutdir)
            os.mkdir(s_fulloutdir)
            self.s_curTrainingCSV = s_fulloutdir+'/'+'vec.csv'
            batchMaker.STATIC_concatCSV_using_numpy(filenames,self.s_curTrainingCSV)
        else:
            self.s_curTrainingCSV = s_fulloutdir+'/'+'vec.csv'

        
        
        #NOTE 이부분에 특정 신경망의 그래프 파일과 ckpt 파일과 dict을 읽어와야 한다. 
        #위 코드는 사용자가 사용하고자 하는 신경망의 이름을 이용해서 
        #trainerusingdict파일의 trainer를 이용하면 된다
        #간단히 trainer에게 두 batchMaker와 신경망의 filename을 제공하면 된다
        #이때 filename은 아마도 절대경로를 다 주어야 할 것이다. 
        
        np2d_train,np2d_test = batchMaker.STATIC_setCSV_splitData_using_numpy(self.s_curTrainingCSV)
        trainBatch,testBatch = batchMaker(),batchMaker()
        trainBatch.setNpArr_using_numpy(np2d_train)
        testBatch.setNpArr_using_numpy(np2d_test)
        s_netname = ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_netname+'/'+s_netname

        self.curTrainer = Trainer(trainBatch,testBatch,s_netname)

        print('preparing Training --> 함수종료!')
        
    def trainSetting(self, byte_payload:bytes):
        #기능 시험완료! 이함수가 멀티프로세스로 돌리면 학습중에 데이터 저장이 가능하겠다. 
        '''
        사용자에게서 훈련에 대한 정보를 받아옵니다.
        가령 배치사이즈 몇으로 몇번씩 몇백번씩 훈련하다가 학습 정보를
        반환할지에 대해서 설정합니다. 
        생각하는 인자는 다음과 같습니다. 
        총훈련 횟수: 10000 
        테스트 반환: 100 회마다 반환
        '''
        data = struct.unpack('3i',byte_payload[:12])
        byte_payload = byte_payload[12:]
        n_batchSize,n_epoch,n_testCycle = data[0],data[1],data[2]

        for epoch in range(n_epoch):
            b_isTrained = self.curTrainer.train(n_batchSize)

            if not b_isTrained:
                self.occurError('batch size is too high. please lower your batch size and retry.')
                print('TCPserver --> trainSetting >> self.occurError 함수 호출 ')
                ##에러 문구를 클라이언트에서 어떻게 받을지에 따라서 다르게 수정하자. 
                break

            if (epoch % n_testCycle == 0):
                acc,loss = self.curTrainer.accuracy_loss(n_batchSize)
                s = 'epoch : '+str(epoch)+' accuracy : '+str(acc)+' loss : '+str(loss)
                print(s)#여기 이부분에서 s를 클라이언트로 전송해야 한다.

                byte_dest = struct.pack('3f',float(epoch/n_epoch),float(acc),float(loss))
                self.sendToClnt(OPCODE_SEND_TRAIN_CUR_PROCESS,len(byte_dest),byte_dest)
                
        #self.curTrainer.store() #나중에는 이곳이 없어져야 한다.
        print('trainSetting --> 함수종료!')

    def predictSetting(self,byte_paylaod:bytes):
        '''
        사용자로부터 특정한 검증 데이터를 가지고 예측을 수행하는 함수
        '''
        pass

    def storeNet(self,byte_payload:bytes):
        '''
        현재 trainer의 신경망 정보를 디스크에 저장하는 함수이다. 
        사용자로부터 새로운 이름을 입력받으면 그 디렉토리를 만들고 저장한다.
        사용자로부터 새로운 입력을 받지 않으면 기존에 파일에 덮어쓴다.

        '''
        n_nameSize = struct.unpack('i',byte_payload[:4])
        byte_payload = byte_payload[4:]
        n_nameSize = n_nameSize[0] #새로 저장할 이름 길이 
        if n_nameSize != 0: #새로 저장할 이름 길이가 0이 아니면
            s_name = struct.unpack(str(n_nameSize)+'c',byte_payload[:n_nameSize])
            byte_payload = byte_payload[n_nameSize:]
            byte_dest = bytes(0)
            for b in s_name[:]:
                byte_dest += b
            s_name = byte_dest.decode()
            if not os.path.exists(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_name):
                os.mkdir(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_name)
            self.curTrainer.store(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_name+'/'+s_name)
        else:
            self.curTrainer.store()

    def createModule(self,byte_paylaod:bytes):
        '''
        페이로드에는 사용자가 모듈을 만들고자 하는 신경망 이름의 길이와
        신경망 이름을 입력으로 필요로 한다. 
        moduleMaker를 이용하여 NETDIR안에 .py파일을 구성한다. 

        모듈을 구성하고 압축파일을 구성하고 페이로드를 구성한다. 
        '''
        n_netSize = struct.unpack('i',byte_paylaod[:4])
        byte_paylaod = byte_paylaod[4:]
        n_netSize = int(n_netSize[0])

        s_netName = struct.unpack(str(n_netSize)+'c',byte_paylaod[:n_netSize])
        byte_paylaod = byte_paylaod[n_netSize:]

        s_netName = (s_netName[:]).decode()
        print('createModule --> s_netName >> ',s_netName)

        if not os.path.exists(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_netName):
            print('그런 신경망은 없습니다. ') #이정보를 사용자에게 전달해야 한다. 
            pass
        else:
            moduleMaker = ModuleMaker(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_netName+'/'+s_netName)
            moduleMaker.writeModule()  # 파이썬 파일을 구성합니다.
            zip = zipfile.ZipFile(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_netName+'.zip','w')
            filenames = os.listdir(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_netName)
            for filename in filenames:
                print('createModule --> filename >> ',filename)
                zip.write(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_netName+'/'+filename,arcname=filename,compress_type=zipfile.ZIP_DEFLATED)
            zip.close()
            print('createModule --> zip.close() 호출')            

        mystat = os.stat(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_netName+'.zip') 
        n_size = mystat.st_size
        n_opcode = OPCODE_REQUESTMODULE   ### 여기 opcode값이 결정되면 수정해야 한다. 

        opener = open(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_netName+'.zip','rb')
        n_remain = n_size
        buf = bytes(0)
        while n_remain >0:
            byte_buf = None
            if n_remain < BUFSIZE:
                byte_buf = opener.read(n_remain)
                n_remain -= n_remain
            else: 
                byte_buf = opener.read(BUFSIZE)
                n_remain -= BUFSIZE
            buf += byte_buf
        return n_opcode,n_size,buf
        
    def prepareSendingData(self,byte_paylaod:bytes):
        '''일단 여기의 필요한 기능은 createModule 함수에 구현하였다. '''
        pass

    def createClassifier(self,byte_payload:bytes):
        # 기능 시험 완료

        n_nameSize = struct.unpack('i',byte_payload[:4])
        byte_payload = byte_payload[4:]
        n_nameSize = int(n_nameSize[0])
        s_name = struct.unpack(str(n_nameSize)+'c',byte_payload[:n_nameSize])
        byte_payload = byte_payload[n_nameSize:]

        byte_dest = bytes(0)
        for b in s_name[:]:
            byte_dest+=b

        s_name = byte_dest.decode() #여기서 에러가 날 수도 있겟다.
        print('createClassifier -> s_name >>',s_name)

        if not os.path.exists(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_name):
            os.mkdir(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_name)
        
        generator = NetClassfierGenerator(byte_payload)
        generator.interpretPayload(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+s_name+'/'+s_name)
        #인자로 신경망디렉토리/신경망이름디렉토리/신경망이름 을 전달한다.

    def connectionTest(self,byte_payload:bytes):
        '''초기 연결 확인용 코드입니다. 클라이언트에서는 현재 사용가능한 사용자 정보를 입력받아야 합니다. '''
        '''기능시험이 필요합니다. '''
        byte_payload = bytes()
        self.mutexlock.acquire()
        #byte_payload += struct.pack('i',len(self.dict_ConnectionInfo))
        n_lenDict = 0
        for key,val in self.dict_ConnectionInfo.items():
            if not val:
                n_lenDict += 1
        byte_payload += struct.pack('i',n_lenDict)

        for key,val in self.dict_ConnectionInfo.items():
            n_strlen = len(key)
            print('TCPserver --> connectionTest >> 디렉토리 이름 >> '+str(key))
            print('TCPserver --> val >> ',val)
            #NOTE: val에 대한 정보 즉 1,0을 보내지 않게 되면 길이로 자를때 문제가 된다. 무조건 이 정보를 보내야 한다.
            if not val:
                print('TCPserver --> connectionTest >> ',key)
                byte_payload += (struct.pack('i',n_strlen) + key.encode())
            else:
                continue
                #byte_payload += (struct.pack('i',n_strlen) + key.encode() + struct.pack('i',0))
        self.mutexlock.release()

        self.sendToClnt(OPCODE_CONNECTION_TEST,len(byte_payload),byte_payload)

    def signUp(self,byte_payload:bytes):
        '''회원가입을 구현한 코드 입니다. '''
        '''기능시험이 필요합니다. '''
        data = struct.unpack('i',byte_payload[:4])
        n_newUserNameLen = data[0]
        byte_payload = byte_payload[4:]

        data = struct.unpack(str(n_newUserNameLen)+'c',byte_payload[:n_newUserNameLen])
        byte_payload = byte_payload[n_newUserNameLen:]
        
        byte_dest = bytes()
        for b in data:
            byte_dest += b

        s_newUserName = byte_dest.decode()
        print('signup --> s_newUserName ',ROOTDIR+'/'+s_newUserName)
        if not os.path.exists(ROOTDIR+'/'+s_newUserName):
            os.mkdir(ROOTDIR+'/'+s_newUserName)
        if not os.path.exists(ROOTDIR+'/'+s_newUserName+'/'+NETDIR):
            os.mkdir(ROOTDIR+'/'+s_newUserName+'/'+NETDIR)
        if not os.path.exists(ROOTDIR+'/'+s_newUserName+'/'+DATADIR):
            os.mkdir(ROOTDIR+'/'+s_newUserName+'/'+DATADIR)
            if not os.path.exists(ROOTDIR+'/'+s_newUserName+'/'+DATADIR+'/'+TRAINSET):
                os.mkdir(ROOTDIR+'/'+s_newUserName+'/'+DATADIR+'/'+TRAINSET)
        if not os.path.exists(ROOTDIR+'/'+s_newUserName+'/'+RESURTDIR):
            os.mkdir(ROOTDIR+'/'+s_newUserName+'/'+RESURTDIR)
        
        self.mutexlock.acquire()
        self.dict_ConnectionInfo[s_newUserName] = False  #사용자 추가
        self.mutexlock.release()
    
    def userExit(self,byte_payload:bytes):                                        ## NOTE: opcode 1000 구현입니다.
        '''클라이언트가 종료 요청을 했을때의 코드입니다. 아마 opcode가 1000 일때의 기능이 수행될 것입니다.
        -1일 경우에는 아직 로그인을 하지 않았을 경우로 예정합니다. 
        따라서 사용자가 connectionTest를 한 경우에는 종료하기 위해서 서버에게 opcode 1000과 lengh 4 (즉 -1의 길이)
        그리고 payload로 -1을 보내야 합니다. '''
        '''기능 시험이 필요합니다. '''

        data = struct.unpack('i',byte_payload[:4])
        n_exituserNameLen = data[0]
        if n_exituserNameLen == -1:
            exit(0)
        else:
            data = struct.unpack(str(n_exituserNameLen)+'c',byte_payload[:n_exituserNameLen])
            s_exituserName = (data[:-1]).decode()
            self.mutexlock.acquire()
            self.dict_ConnectionInfo[s_exituserName] = False #False이면 접속하고있지 않음
            self.mutexlock.release()
            print('TCPserver --> userExit >> self.dict_ConnectionInfo >> ',self.dict_ConnectionInfo)
            self.tcpClientSocket.close()
            print('TCPserver --> userExit >> 클라이언트 접속 종료! ')
            exit(0) #쓰레드 종료!

    def listCurUserDataDirInfo(self):                                       ## NOTE: opcode 13 구현입니다. 
        if self.s_username == None:
            print('TCPserver --> listCurUserDataDirInfo >> s_username이 None입니다!')
            pass
        li_dirinfo = os.listdir(ROOTDIR+'/'+self.s_username+'/'+DATADIR)
        li_dirinfo = [filename for filename in li_dirinfo if not filename[-4:] == '.zip']

        byte_dest = bytes()
        byte_dest += struct.pack('i',(len(li_dirinfo)))

        for dirname in li_dirinfo:
            if dirname == VECDIR: #VECDIR은 bird안의 vec0 vec1이 들어있는 디렉토리
                print('확인용 listCurUserDataDirInfo --> dirname >> ',dirname) #정상확인위해 출력
                continue
            byte_dirname = dirname.encode()
            byte_dest += struct.pack('i',len(byte_dirname))
            byte_dest += byte_dirname

            li_Files = os.listdir(ROOTDIR+'/'+self.s_username+'/'+DATADIR+'/'+dirname+'/')
            byte_dest += struct.pack('i',len(li_Files))

        self.sendToClnt(OPCODE_LISTDATADIR,len(byte_dest),byte_dest)

    def listCurUserNetDirInfo(self):                                        ## NOTE: opcode 14 구현 입니다.
        '''
        신경망 디렉토리 순회코드 구현입니다. 기능시험이 필요합니다.
        서버/유저/신경망디렉토리/ 를 조회합니다.
        그리고 신경망1 신경망2 신경망3 에 대해서
        바이너리로 저장한 신경망 구성파일(페이로드 파일)을 읽어들입니다. 
        그리고 레이블 정보를 저장하고 있는 파일을 가지고 있다면 그 데이터도 전달합니다. 
        페이로드는 다음과 같습니다.
        
        신경망디렉토리갯수 + 디렉토리이름길이 + 디렉토리이름 + 신경망구성바이너리길이 + 신경망구성바이너리
        만약 레이블 정보가 없다면  + (-1)    <-- 0으로 하고 싶었는데.. 신경망에서 사용하는레이블 정보가 0부터 시작해서....
        만약 레이블 정보가 있다면  + 레이블의갯수(bird, cat, dog 라면 3) + 레이블 정보(int) + 레이블이름길이 + 레이블 이름

        위와같은것이 반복됩니다. 엄청 길겠네요 ㄷㄷ; 차라리 압축해서 보낼까요 
        일단 구현을 다 한 다음에 나중에 sendToServ recvFromServ 등에서 일괄 압축해서 전달하는 것도 좋을것 같습니다. 
        그러면 클라의 하드웨어 환경에 따라서 네트워크 부터 느려지겠군요 ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ
        '''
        if self.s_username == None:
            print('TCPserver --> listCurUserNetDirInfo >> s_username이 None입니다!')
            pass
        li_dirinfo = os.listdir(ROOTDIR+'/'+self.s_username+'/'+NETDIR)
        byte_dest = bytes()
        byte_dest += struct.pack('i',len(li_dirinfo))   #전체 디렉토리의 갯수 전달 
        for dirname in li_dirinfo:
            listdirs = os.listdir(ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+dirname+'/')
            payloadFileName = ''
            labelFileName = ''
            for tmp in listdirs:
                if tmp[-len(PAYLOADBIN):] == PAYLOADBIN: #뒷글자가 페이로드바이너리파일의 이름과 같다면 
                    payloadFileName = tmp
                if tmp[-len(LABELINFO):] == LABELINFO:
                    labelFileName = tmp
            print('[디버깅용] TCPserver --> lsitcurUserNetDirInfo >> payloadFileName >> ',payloadFileName)
            print('[디버깅용] TCPserver --> lsitcurUserNetDirInfo >> labelFileName >> ',labelFileName)

            s_curNetPath = ROOTDIR+'/'+self.s_username+'/'+NETDIR+'/'+dirname
            #netstat = os.stat(s_curNetPath+'/'+payloadFileName)
            #fileSize = netstat.st_size
            f = open(s_curNetPath+'/'+payloadFileName,'rb')
            payload = f.readline()
            f.close()
            #NOTE: 여기서 stat으로 읽어온 파일의 크기와 read binary해서 읽어온 파일의 크기가 같은지 확인해야 합니다!
            byte_dirname = dirname.encode()
            byte_dest += struct.pack('i',len(byte_dirname)) #디렉토리의 이름 길이 저장
            byte_dest += byte_dirname #디렉토리의 이름저장 
            #byte_dest += struct.pack('i',fileSize) #신경망 구성 페이로드의 길이 저장
            byte_dest += payload #신경망 구성 페이로드 저장 

            #레이블 정보를 저장합니다. 숫자레이블 + 레이블이름길이 + 레이블이름  의 순서로 전달 됩니다. 
            if labelFileName != '': # 이렇다면 이전에 학습이 되어 훈련데이터에 종속된 신경망
                byte_dest += struct.pack('i',1)  ##레이블 정보를 가지고 있다면 1전송
                dict_labelInfo = dictSaver().import_dict(s_curNetPath+'/'+labelFileName)
                byte_dest += struct.pack('i',len(dict_labelInfo)) #레이블 갯수를 전달합니다. 
                for n_label,s_labelname in dict_labelInfo.items():
                    byte_labelname = s_labelname.encode()
                    byte_dest += (struct.pack('2i',n_label,len(byte_labelname)) + byte_labelname)
                    #레이블(0,1,2, 등등등..) + 레이블이름길이 + 레이블이름 
            else:
                byte_dest += struct.pack('i',0)
                #레이블정보를 가지고 있지 않다면 0을 전달합니다.

        self.sendToClnt(OPCODE_LISTNETDIR,len(byte_dest),byte_dest)

    def listCurUserNetDirInfoNoData(self):                                      # NOTE: opcode 16입니다. 
        if self.s_username == None:
            print('TCPserver --> listCurUserNetDirInfo >> s_username이 None입니다!')
            pass
        li_dirinfo = os.listdir(ROOTDIR+'/'+self.s_username+'/'+NETDIR)
        byte_dest = bytes()
        byte_dest += struct.pack('i',len(li_dirinfo))
        for dirname in li_dirinfo:
            byte_dirname = dirname.encode()
            byte_dest += struct.pack('i',len(byte_dirname))
            byte_dest += byte_dirname

        self.sendToClnt(OPCODE_LISTNETDIRNODATA,len(byte_dest),byte_dest) ## 16

    def listCurUserResultDirInfo(self):                                         # NOTE: opcode 15입니다. 
        if self.s_username == None:
            print('TCPserver --> listCurUserResultDirInfo >> s_username이 None입니다!')
            pass
        li_dirinfo = os.listdir(ROOTDIR+'/'+self.s_username+'/'+RESURTDIR)
        byte_dest = bytes()
        byte_dest += len(li_dirinfo)
        for dirname in li_dirinfo:
            byte_dirname = dirname.encode()
            byte_dest += struct.pack('i',len(byte_dirname))
            byte_dest += byte_dirname

        self.sendToClnt(OPCODE_LISTRESULTDIR,len(byte_dest),byte_dest)

    def sendImgZipFileToClnt(self,byte_paylaod:bytes):                          # NOTE: opcode 17입니다. 
        '''zip파일을 전달하는 함수입니다. 기능 시험이 필요합니다. '''
        n_zipFileNum = struct.unpack('i',byte_paylaod[:4])[0] #파일 갯수 
        byte_paylaod = byte_paylaod[4:]
        byte_dest = struct.pack('i',n_zipFileNum)
        for idx in range(n_zipFileNum):
            n_len = struct.unpack('i',byte_paylaod[:4])[0] #요청한 파일의 이름 길이
            byte_paylaod = byte_paylaod[4:]
            s_name = struct.unpack(str(n_len)+'c',byte_paylaod[:n_len])
            byte_paylaod = byte_paylaod[n_len:]
            s_name = b''.join(s_name).decode()+'.zip'
            if not os.path.exists(ROOTDIR+'/'+DATADIR+'/'+s_name): 
                print('TCPserver --> sendImgZip >> 파일이 존재하지 않습니다. ')
                self.occurError(s_name+'란 파일은 없습니다!')
            mystat = os.stat(ROOTDIR+'/'+DATADIR+'/'+s_name)
            zipsize = mystat.st_size
            f = open(ROOTDIR+'/'+DATADIR+'/'+s_name,'rb')
            byte_zipbufer = f.read(zipsize)
            f.close()
            byte_dest += (struct.pack('i',zipsize)+byte_zipbufer)
        print('[디버깅용]TCPserver --> sendImgZip >> 데이터파일 구성 완료 전송 대기 ')
        self.sendToClnt(OPCODE_SENDIMGZIP,len(byte_dest),byte_dest)

    def occurError(self,s_errorInfo:str):
        '''에러 전달 함수 입니다.'''
        '''기능시험이 필요합니다. '''
        n_errorLen = len(s_errorInfo)
        byte_dest = struct.pack('i',n_errorLen)
        byte_dest += s_errorInfo.encode()
        print('디버깅용 TCPserver --> occurError >> s_errorInfo ',s_errorInfo)
        self.sendToClnt(OPCODE_ERROR,len(byte_dest),byte_dest)

    @staticmethod
    def testloop():
        '''테스트를 위한 루프를 만듭니다.'''
        train,test = batchMaker.STATIC_setCSV_splitData_using_numpy('./server/david/data/sbd/test.csv')
        train_batch,test_batch = batchMaker(),batchMaker()
        train_batch.setNpArr_using_numpy(train)
        test_batch.setNpArr_using_numpy(test)
        curTrainer = Trainer(train_batch,test_batch,'./server/david/net/testnet2')

        curTrainer.testfunc(20)

        pass

    def run(self):
        '''서버의 메인 루프를 나타내는 함수입니다.'''

        while True:
            n_opcode,n_size,byte_payload = self.recvFromClnt()
            print('opcode >> ',n_opcode)

            if n_opcode == 0:
                #사용자의 첫 접속일 때 동작 
                #사용자의 이름으로 디스크에 디렉토리를 구성한다.
                self.nameSetting(byte_payload)
                self.dirSetting()

                print('TCPserver.run --> nameSetting, dirSetting 완료')

            elif n_opcode == OPCODE_CREATECLF:
                #분류기 구성 정보 전송
                self.createClassifier(byte_payload)

                print('TCPserver.run --> createClassifier 완료')

            elif n_opcode == OPCODE_STOREDATA:
                #이미지 압축파일 전송
                self.storeData(byte_payload)

                print('TCPserver.run --> storeData 그러나 이는 프로세스가 완료되어야 합니다')

            elif n_opcode == OPCODE_PRETRAINING:
                #학습 하고자 하는 분류기와 훈련 데이터 선택
                self.prepareTraining_seperate(byte_payload)

                print('TCPserver.run --> prepareTraining 완료')

            elif n_opcode == OPCODE_TRAINSET:
                #분류기 학습 
                self.trainSetting(byte_payload)

                print('TCPserver.run --> trainSetting 완료')

            elif n_opcode == OPCODE_STORECURNET:
                #학습정보 반환인데 이것이 아마 <분류기학습>에서 할수도 있다!
                self.storeNet(byte_payload)

            elif n_opcode == OPCODE_REQUESTMODULE:
                #모델 반환 즉 이는 모델의 압축파일을 사용자에서 서버에 요청하는 것
                #기능시험 필요
                n_co,n_si,byte_pay = self.createModule(byte_payload)
                self.sendToClnt(n_co,n_si,byte_pay)
                
            elif n_opcode == 7:
                #모델을 전송 모델의 압축파일을 서버에서 사용자에게 보내는것 
                #이는 클라이언트에서 구현되어야 할 것이다. 
                pass

            elif n_opcode == 8:
                #현재 모델을 디스크에 저장하는 것입니다. 사용자가 새로운 이름을 전달한다면 그것에 맞게 새로운 파일로 저장됩니다. 
                self.storeData(byte_payload)

            elif n_opcode == OPCODE_CONNECTION_TEST:
                #연결테스트를 하는 곳입니다. 클라이언트에서 바로 네트워크를 끊기를 기대합니다. 
                self.connectionTest(byte_payload)

            elif n_opcode == OPCODE_CONNECTION:
                #CODE_CONNECTION 11 
                self.nameSetting(byte_payload)
                self.dirSetting()
            
            elif n_opcode == OPCODE_SIGNUP:
                #CODE_SIGNUP 12
                self.signUp(byte_payload)

            elif n_opcode == OPCODE_LISTDATADIR:  #13
                self.listCurUserDataDirInfo()

            elif n_opcode == OPCODE_LISTNETDIR:  #14
                self.listCurUserNetDirInfo()

            elif n_opcode == OPCODE_LISTRESULTDIR:  #15 
                self.listCurUserResultDirInfo()

            elif n_opcode == OPCODE_LISTNETDIRNODATA:  #16 
                self.listCurUserNetDirInfoNoData()

            elif n_opcode == OPCODE_SENDIMGZIP:    #17
                self.sendImgZipFileToClnt(byte_payload)

            elif n_opcode == OPCODE_ERROR:
                #오류메시지 전송으로 서버에서 사용자 사용자에서 서버로 전송하는 것
                
                pass

            elif n_opcode == 101:
                #gpu가 이미 할당되어 있다는 에러 메시지를 서버에서 사용자로 전송하는 것
                pass

            elif n_opcode == OPCODE_EXIT:
                #CODE_EXIT
                self.userExit(byte_payload)

        s_modelname = './joljak/mo'
        byte_data = self.tcpClientSocket.recv(BUFSIZE)

        generator = NetClassfierGenerator(byte_data) 
        generator.interpretPayload(s_modelname)

        np2d_train,np2d_test = batchMaker.STATIC_setCSV_splitData(self.filename)
        train,test = batchMaker(),batchMaker()
        train.setNparr(np2d_train)
        test.setNparr(np2d_test)

        handler = Trainer(train,test,s_modelname)

        for i in range(1000):
            handler.train(n_batchSize = 30)
            f_accuracy,f_loss = handler.accuracy_loss(30)
            
        np1d_argmax = handler.predict(30)

        self.tcpClientSocket.close()

class TCPlistener():
    n_client = 0
    def __init__(self,tu_addr = ADDR,n_client = 50):
        self.sock_TCPserver = socket(AF_INET,SOCK_STREAM)
        self.sock_TCPserver.bind(tu_addr)
        self.sock_TCPserver.listen(n_client)
        ###328 추가 
        li_dir = os.listdir(ROOTDIR+'/')

        self.dict_userConnectionInfo = {dirname:False for dirname in li_dir}
        print('TCPlistener --> __init__ >> ',self.dict_userConnectionInfo)
        #NOTE: 사용자가 온라인 상태라면 True 로 설정하자.
        self.mutex = threading.Lock()

    def work(self): #서버의 리스너 소켓의 구현부
        while True:
            try:
                print('server is ready to receive')
                sock_client,addr = self.sock_TCPserver.accept()                
                TCPserver(sock_client,self.mutex,self.dict_userConnectionInfo).start()
                TCPlistener.n_client += 1
                print('n_client : ',TCPlistener.n_client)
            except Exception as e:
                print('exception in Listener >> ',e)
                continue

    def __del__(self):
        self.sock_TCPserver.close()

def main():
    listener = TCPlistener()
    listener.work()

def testmain():
    TCPserver.testloop()
    
if __name__ == "__main__":
    main()


'''
data = bytes(0)
                n_opAndPaySize = 4
                while True:
                    tmp = sock_client.recv(n_opAndPaySize - len(data))
                    data += tmp
                    if len(data) == n_opAndPaySize: break
                    time.sleep(1) #busy waiting을 간단히 막는 방법 적당히 동작함
                n_recvdata = struct.unpack('i',data[:4])
                print('TCPserver --> n_recvdata >> ',n_recvdata[0])
                byte_senddata = struct.pack('i',n_recvdata[0])
                sock_client.send(byte_senddata)

                n_dictlen = len(self.dict_userConnectionInfo)
                print(n_dictlen)
                byte_senddata = struct.pack('i',n_dictlen)
                sock_client.send(byte_senddata)


'''