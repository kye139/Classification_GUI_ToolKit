import numpy as np
import tensorflow as tf
import os
from TrainerUsingDict import Trainer

class ModuleMaker():

    def __init__(self,s_path:str):
        self.s_filename = s_path
        li_tmp = s_path.split('/')
        self.s_netname = li_tmp[-1]

    def writeModule(self):
        '''파이썬 파일을 구성합니다.'''
        f = open(self.s_filename+'.py','w')
        end = '\n'
        s = ''
        s+="import tensorflow as tf"+end
        s+="import numpy as np"+end
        s+="import os"+end
        s+="from dictSaver import dictSaver"+end
        s+="class NetClassifier():"+end
        s+="    def __error__(self,s_error:str):"+end
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
        
        s+="    def predict(self,filepath:str,np2d_label):"+end
        s+="        "
        s+="        feed_dict =  {self.dict_tsdata['input']:np2d_input,self.dict_tsdata['label']:np2d_label,self.dict_tsdata['trainable']:False}"+end
        s+="        np1d_argmax = self.tfsess.run(self.dict_tsdata['predict'],feed_dict = feed_dict)"+end
        s+="        return np1d_argmax"+end


        f.write(s)
        f.close()

    def writeSettingModule(self):
        '''
        압축해제를 한후에 파일들을 C:/clf/로 이동시켜서 당장 사용할 수 있도록 설정해주는 파일을 생성
        가능하다면 pyinstaller로 exe도 만들어 보겠는데 제대로 될지는 모르겠음

        '''
        pass

