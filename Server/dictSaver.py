

class dictSaver():
    '''
    dictSaver클래스는 신경망의 정보를 저장하고 있는 딕셔너리를 다루는 클래스다.
    신경망에서 주요 노드를 가지고 있는 딕셔너리를 savd_dict 함수로 받아 저장한다. 
    위에서 저장한 딕셔너리를 불러올때는 import_dict에 파일이름을 주면 된다. 
    이렇게 구성된 딕셔너리를 trainer클래스에 제공하면 중요 노드를 파악할 수 있다.
    '''
    def __init__(self):
        pass

    def save_dict(self,dict_input:dict,filename:str):
        '''
        현재 신경망의 정보를 나타내는 딕셔너리를 구분자 |를
        이용하여 텍스트 파일에 저장한다. 
        '''
        filehandler = open(filename,'w')
        for s_key, s_value in dict_input.items():
            s_tmp = s_key+'|'+s_value+'\n'  
            filehandler.write(s_tmp)
        filehandler.close()
        
    def import_dict(self,filename:str):
        '''
        filename을 입력으로 받아서 딕셔너리를 구성하는 함수이다.

        '''
        fileHandler = open(filename,'r')
        mydict = dict()
        while True:
            line = fileHandler.readline()
            if not line: break
            line = line.split('|') 
            #이곳에서 \n도 같이 들어갈수도 있겠다. 확인해봐야하겠다.
            #만일 그렇다면 '\n'을 제외시킬수 있도록 line[1][:-1]과 같이 해보자
            mydict[line[0]] = line[1][:-1] 
        return mydict
