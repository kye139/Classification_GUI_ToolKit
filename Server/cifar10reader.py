import numpy as np
import cv2
from multiprocessing import Process, Value, Array, Lock
import pickle
import os

DIR_AIRPLANE = './joljak/bigsize/airplane'
DIR_AUTOMOBILE = './joljak/bigsize/automobile'
DIR_BIRD = './joljak/bigsize/bird'
DIR_CAT = './joljak/bigsize/cat'
DIR_DOG = './joljak/bigsize/dog'
DIR_DEER = './joljak/bigsize/deer'
DIR_FROG = './joljak/bigsize/frog'
DIR_HORSE = './joljak/bigsize/horse'
DIR_SHIP = './joljak/bigsize/ship'
DIR_TRUCK = './joljak/bigsize/truck'

def dirSetting():
    if not os.path.exists('./joljak/bigsize'):
        os.mkdir('./joljak/bigsize')
    if not os.path.exists(DIR_AIRPLANE):
        os.mkdir(DIR_AIRPLANE)
    if not os.path.exists(DIR_AUTOMOBILE):
        os.mkdir(DIR_AUTOMOBILE)
    if not os.path.exists(DIR_BIRD):
        os.mkdir(DIR_BIRD)
    if not os.path.exists(DIR_CAT):
        os.mkdir(DIR_CAT)
    if not os.path.exists(DIR_DOG):
        os.mkdir(DIR_DOG)
    if not os.path.exists(DIR_DEER):
        os.mkdir(DIR_DEER)
    if not os.path.exists(DIR_FROG):
        os.mkdir(DIR_FROG)
    if not os.path.exists(DIR_HORSE):
        os.mkdir(DIR_HORSE)
    if not os.path.exists(DIR_SHIP):
        os.mkdir(DIR_SHIP)
    if not os.path.exists(DIR_TRUCK):
        os.mkdir(DIR_TRUCK)

def writeCifarImg(s_path,lock,l_cnt):
    #l_cnt는 공유메모리의 리스트
    f = open(s_path,'rb')
    mydict = pickle.load(f,encoding='bytes')
    f.close()
    data = mydict[b'data']
    labels = mydict[b'labels']

    row,col = data.shape

    for r in range(row):
        img = data[r]
        R = img[0:1024].reshape(32,32)
        G = img[1024:2048].reshape(32,32)
        B = img[2048:].reshape(32,32)
        img = np.dstack((B,G,R))
        img = img.astype(np.uint8)
        img = cv2.resize(img,(320,320),interpolation=cv2.INTER_LINEAR)
        n = int(labels[r])
        lock.acquire()
        if n == 0:
            cv2.imwrite(DIR_AIRPLANE+'/airplane'+str(l_cnt[n].value)+'.jpg',img)
            l_cnt[n].value += 1
        elif n == 1:
            cv2.imwrite(DIR_AUTOMOBILE+'/automobile'+str(l_cnt[n].value)+'.jpg',img)
            l_cnt[n].value += 1
        elif n == 2:
            cv2.imwrite(DIR_BIRD+'/bird'+str(l_cnt[n].value)+'.jpg',img)
            l_cnt[n].value += 1
        elif n == 3:
            cv2.imwrite(DIR_CAT+'/cat'+str(l_cnt[n].value)+'.jpg',img)
            l_cnt[n].value += 1
        elif n == 4:
            cv2.imwrite(DIR_DEER+'/deer'+str(l_cnt[n].value)+'.jpg',img)
            l_cnt[n].value += 1
        elif n == 5:
            cv2.imwrite(DIR_DOG+'/dog'+str(l_cnt[n].value)+'.jpg',img)
            l_cnt[n].value += 1
        elif n == 6:
            cv2.imwrite(DIR_FROG+'/frog'+str(l_cnt[n].value)+'.jpg',img)
            l_cnt[n].value += 1
        elif n == 7:
            cv2.imwrite(DIR_HORSE+'/horse'+str(l_cnt[n].value)+'.jpg',img)
            l_cnt[n].value += 1
        elif n == 8:
            cv2.imwrite(DIR_SHIP+'/ship'+str(l_cnt[n].value)+'.jpg',img)
            l_cnt[n].value += 1
        elif n == 9:
            cv2.imwrite(DIR_TRUCK+'/truck'+str(l_cnt[n].value)+'.jpg',img)
            l_cnt[n].value += 1
        lock.release()


def main():
    l = [Value('i',0),Value('i',0),Value('i',0),Value('i',0),Value('i',0),\
        Value('i',0),Value('i',0),Value('i',0),Value('i',0),Value('i',0)]
    lock = Lock()
    dirSetting()

    p1 = Process(target=writeCifarImg,args=('./joljak/cifar-10-python.tar/data/data_batch_1',lock,l))
    p2 = Process(target=writeCifarImg,args=('./joljak/cifar-10-python.tar/data/data_batch_2',lock,l))
    p3 = Process(target=writeCifarImg,args=('./joljak/cifar-10-python.tar/data/data_batch_3',lock,l))
    p4 = Process(target=writeCifarImg,args=('./joljak/cifar-10-python.tar/data/data_batch_4',lock,l))
    p5 = Process(target=writeCifarImg,args=('./joljak/cifar-10-python.tar/data/data_batch_5',lock,l))
    
    p1.start(),p2.start(),p3.start(),p4.start(),p5.start()

if __name__ == "__main__":
    main()