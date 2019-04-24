import numpy as np
import tensorflow as tf
from batchMaker import *

class Trainer():
    def __init__(self,tfplc_input_,tfplc_label_,tfplcdft_b_training_,tfts_train_,tfts_accuracy_,tfts_loss_,tfts_argmax_,train_batch_,test_batch_):
        # 지금은 편의상 이렇게 구현했으나 후에 딕셔너리로 수정할 것
        self.train_batch = train_batch_
        self.test_batch = test_batch_
        self.tfsess_sess = tf.Session() #노드들을 평가하기 위한 세션
        self.tfplc_input = tfplc_input_ #입력층의 플래이스홀더
        self.tfplc_label = tfplc_label_ #라벨층의 플레이스 홀더
        self.tfplcdft_b_training = tfplcdft_b_training_ #훈련중에만 사용될 텐서를 위한 bool형 플레이스홀더
        self.tfts_train = tfts_train_ #훈련을 위한 텐서 
        self.tfts_accuracy = tfts_accuracy_ #정확도 표현을 위한 텐서
        self.tfts_loss = tfts_loss_ #오차표현을 위한 텐서 
        self.tfts_argmax = tfts_argmax_ #예측을 위한 텐서
    
    def predict(self,n_batchSize,np2d_input = None,np2d_label = None):
        #None이 아니면 사용자가 원하는 검증 데이터가 들어올 것으로 예상한다.
        feed_dict = None

        if np2d_input != None and np2d_label != None: #사용자가 원하는 데이터를 입력한경우
            feed_dict = {self.tfplc_input:np2d_input,self.tfplc_label:np2d_label,self.tfplcdft_b_training:False}
        else: #Trainer에게 입력으로 주었던 배치 메이커를 사용하는 경우
            data,label = self.test_batch.next_batch(n_batchSize)
            feed_dict = {self.tfplc_input:data,self.tfplc_label:label,self.tfplcdft_b_training:False}

        np1d_argmax = self.tfsess_sess.run(self.tfts_argmax,feed_dict = feed_dict)
        return np1d_argmax

    def train(self,n_batchSize,np2d_input = None,np2d_label = None):
        #None이 아니면 사용자가 원하는 검증 데이터가 들어올 것으로 예상한다.
        feed_dict = None

        if np2d_input != None and np2d_label != None: #사용자가 원하는 데이터를 입력한경우
            feed_dict = {self.tfplc_input:np2d_input,self.tfplc_label:np2d_label,self.tfplcdft_b_training:True}
        else: #Trainer에게 입력으로 주었던 배치 메이커를 사용하는 경우
            data,label = self.train_batch.next_batch(n_batchSize)
            feed_dict = {self.tfplc_input:data,self.tfplc_label:label,self.tfplcdft_b_training:True}

        self.tfsess_sess.run(self.tfts_train,feed_dict = feed_dict)

    def accuracy_loss(self,n_batchsize,np2d_input = None,np2d_label = None):
        feed_dict = None

        if np2d_input != None and np2d_label != None: #사용자가 원하는 데이터를 입력한경우
            feed_dict = {self.tfplc_input:np2d_input,self.tfplc_label:np2d_label,self.tfplcdft_b_training:True}
        else: #Trainer에게 입력으로 주었던 배치 메이커를 사용하는 경우
            data,label = self.test_batch.next_batch(n_batchsize)
            feed_dict = {self.tfplc_input:data,self.tfplc_label:label,self.tfplcdft_b_training:True}

        
        nparr_accuracy,nparr_loss = self.tfsess_sess.run([self.tfts_accuracy,self.tfts_loss],feed_dict = feed_dict)        
        return nparr_accuracy,nparr_loss
    
    
    
    
    