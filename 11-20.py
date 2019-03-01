import sensor, image, time ,math#引入摄像头、图像、时间模块
from pid import PID #引入PID模块
from pyb import UART #引入UART串口通信模块

sensor.reset() #初始化摄像头
sensor.set_pixformat(sensor.RGB565) #设置RGB图像格式
sensor.set_framesize(sensor.QQVGA) #设置图像分辨率
sensor.skip_frames(10,1000) #跳过10帧
sensor.set_contrast(1) #设置对比度
sensor.set_auto_whitebal(False) #关闭白平衡
#sensor.set_auto_gain(False) #关闭自动增益

time.sleep(2000) #等待2S

white_thresholds = (62, 98, 1, 9, -15, 10) #设置白色阈值
red_thresholds   =  (41, 83, 24, 83, -18, 60)#设置红色阈值
green_thresholds = (47, 93, -69, -24, 26, 83) #设置绿色阈值
threshold_change = red_thresholds #颜色阈值变量


size_threshold1 = 2500 #设置色块大小
size_threshold2 = 4000 #设置色块大小
x_pid = PID(p=4.9, i=0.5, imax=100) #初始化P、I参数
h_pid = PID(p=0.12, i=0.019 , imax=50) #初始化P、I参数

uart = UART(3,115200) #设置通信串口P4、P5

K = 500 #计算距离系数
logo = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0] #判断距离大小是否趋于稳定的标识符
           #             /               /              /               /               /             /
k = 0 #计数变量
count = 0 #计数变量
count_1 = 0 #计数变量

flag = 1 #判断识别哪种颜色的标识符 初始值为1
flag1 = 0 #判断是否执行green_position(img)函数标识符 初始值为0

green_logol = 2 #转向标识符
action = 1 #是否执行判断绿色区域在视野中的位置的标识符

come_in = 1 #初始化come_in数值

turn = 1 #转化标识符
number=0

#计算出最大色块函数
def find_max(blobs):
    max_pixels=0
    for blob in blobs:
        if blob[4] > max_pixels:
            max_blob=blob
            max_pixels = blob[4]
    return max_blob

#h[3],w[2]
#判断多球
def Multi_ball(blobssss):
    max_blob=find_max(blobssss)
    _,_,w,h=max_blob.rect()
    percent=math.fabs(w-h)/float(h)*100
    #print(percent)
    if(percent>15):
        return 1#有多球
    else:
        return 0#只有一球

#判断绿色区域在视野的位置
def green_position(img):
    blobs_1 = img.find_blobs([green_thresholds],pixels_threshold = 100,area_threshold = 100,x_stride=2, y_stride=2,merge = 1)
    if blobs_1:
        max_blob = find_max(blobs_1)
        if (max_blob[5]<img.width()/2):
            #print(max_blob[5]-img.width()/2)
            return 1#向右转标识符
        else:
            #print(max_blob[5]-img.width()/2)
            return -1#向左转标识符
    else:
      return green_logol

#避免储存区周围球
def avoid_green(img,blob_1):
    global green_logol,turn
    blobs_2 = img.find_blobs([green_thresholds],pixels_threshold = 100,area_threshold = 100,x_stride=2, y_stride=2,merge = 1)
    if blobs_2: #在找到目标球的情况下，避免绿色区域
        max_blob = find_max(blobs_2)
        lvaule=max_blob[0]
        #print(lvaule)
        if(lvaule < 0):
            lvaule = 0
        rvaule=max_blob[0]+max_blob[2]
        #print(rvaule)
        if(rvaule > 160):
            rvaule = 160
        if ( blob_1[5] > lvaule and blob_1[5] < rvaule ):#可直接冲过去去抓
            turn = 1 #turn恢复原始值1
            return 1#经过pid
        else:#反方向转动，寻找小球
            if(turn == 1):
                green_logol= green_logol*(-1)
            return 0#不经过pid
    else: #没有绿色区域，继续原来的方向打转
        return 1#经过pid

#寻找色块函数
def find_target(threshold):
    global green_logol,action,come_in,turn,number
    if number:
        number=0
        a = int(666) # 得出输入到左轮电机值
        b = int(666) # 得出输入到右轮电机值
        c = 'L' + str(a) +'R' + str(b) + 'S' #转化字符串
        c = c.encode("utf-8") #将字符串转化为“utf-8”格式【转化为字节流】
        uart.write(c) #发送字节流
        return 6
    img = sensor.snapshot().binary([white_thresholds], invert=False, zero=True) #获取一帧图片，并将图片上的白色强光去除
    if (threshold == red_thresholds):
        blobs = img.find_blobs([threshold],pixels_threshold = 10,area_threshold = 10,x_stride=2, y_stride=2,merge = 0) #获取红色色块列表
    else:
        blobs = img.find_blobs([threshold],pixels_threshold = 200,area_threshold = 200,x_stride=2, y_stride=2,merge = 1) #获取绿色色块列表
    if blobs:
        if( action == 1 ):
            action = 0 #保证一个大循环程序执行一次此函数
            green_logol = green_position(img)
            #print(green_logol)

        if(flag1==1 and threshold == red_thresholds ):
                blobss = find_max(blobs)
                come_in = avoid_green(img,blobss)
                #print(come_in)
                if(come_in == 0):
                    turn = 2
                    if(green_logol ==1 or green_logol == 2 ):
                        a = int(200)  #得出输入到左轮电机值
                        b = int(-200) #得出输入到右轮电机值
                        c = 'L' + str(a) +'R' + str(b) + 'S' #转化字符串
                        c = c.encode("utf-8") #将字符串转化为“utf-8”格式【转化为字节流】
                        uart.write(c) #发送字节流
                    else:
                        a = int(-200)  #得出输入到左轮电机值
                        b = int(200) #得出输入到右轮电机值
                        c = 'L' + str(a) +'R' + str(b) + 'S' #转化字符串
                        c = c.encode("utf-8") #将字符串转化为“utf-8”格式【转化为字节流】
                        uart.write(c) #发送字节流
                    return 0


                if(turn==2 and come_in == 1):
                    time.sleep(500)
                    turn = 1 #turn恢复原始值1
                    img = sensor.snapshot().binary([white_thresholds], invert=False, zero=True)
                    blobsss = img.find_blobs([red_thresholds],pixels_threshold = 10,area_threshold = 10,x_stride=2, y_stride=2,merge = 0)
                    if( len(blobsss) == 0):
                        return 0
                    else:
                        blobs=blobsss
                        #print(111111111)


        if( (come_in or threshold == green_thresholds) and (len(blobs)!=0) ):
            #print(1000)
            max_blob = find_max(blobs) #获取最大色块
            x_error = max_blob[5]-img.width()/2 #计算“x_error”值
            if(threshold == red_thresholds):
                h_error = max_blob[2]*max_blob[3]-size_threshold1 #计算“h_error”值
                print(h_error)
            else:
                h_error = max_blob[2]*max_blob[3]-size_threshold2 #计算“h_error”值

            img.draw_rectangle(max_blob[0:4]) #绘制矩形
            img.draw_cross(max_blob[5], max_blob[6]) #绘制中心十字架
            x_output =x_pid.get_pid(x_error,1) #计算“x_output”值
            h_output=h_pid.get_pid(h_error,1) #计算“h_output”值
            if( threshold == green_thresholds and (h_error > (-100) and h_error < 9500) ):
                a = int((-x_output)) # 得出输入到左轮电机值
                b = int((+x_output)) # 得出输入到右轮电机值
            else:
                a = int((-h_output-x_output)) # 得出输入到左轮电机值
                b = int((-h_output+x_output)) # 得出输入到右轮电机值
            #限制a的输出范围为-255~255
            if(a < -255 or a > 255):
                if a< -255:
                    a = -255
                else:
                    a = 255
            #限制b的输出范围为-255~255
            if(b < -255 or b > 255):
                if b< -255:
                    b = -255
                else:
                    b = 255
            c = 'L' + str(a) +'R' + str(b) + 'S' #转化字符串
            c = c.encode("utf-8") #将字符串转化为“utf-8”格式【转化为字节流】
            uart.write(c) #发送字节流
            Lm = (max_blob[2]+max_blob[3])/2 #计算求出距离值的系数
            length = K/Lm #计算距离值
            return int(length) #返回距离值
    else:
        if(green_logol == 2):
            a = int(200)  #得出输入到左轮电机值
            b = int(-200)  #得出输入到右轮电机值
        else:
            if( green_logol ==-1 ): #turn left
                a = int(-200) # 得出输入到左轮电机值
                b = int(200) # 得出输入到右轮电机值
            else:
                if( green_logol ==1 ):#turn right
                    a = int(200) # 得出输入到左轮电机值
                    b = int(-200) # 得出输入到右轮电机值
        c = 'L' + str(a) +'R' + str(b) + 'S' #转化字符串
        c = c.encode("utf-8") #将字符串转化为“utf-8”格式【转化为字节流】
        uart.write(c) #发送字节流
        return 0


while(True):
    logo[k] = find_target(threshold_change)
    k+=1
    if(k == 30): #调试时修改！！！！
        k = 0
        count = 1
        for i in range(29): #调试时修改！！！！
            if ( logo[0]==logo[i+1] and logo[0] != 0 ) :
                count = count+1
                if(count == 2):
                    count_1 = count_1 + 1
    if(count == 30 or count_1 >20): #调试时修改！！！！
        #print(count)
        #print(count_1)
        count = 1
        count_1 = 0
        if(threshold_change == green_thresholds):
            flag = 0
        if(flag == 1):
            img = sensor.snapshot().binary([white_thresholds], invert=False, zero=True)
            blobssss = img.find_blobs([red_thresholds],pixels_threshold = 10,area_threshold = 10,x_stride=2, y_stride=2,merge = 0)
            if(Multi_ball(blobssss)):
                number=1
                find_target(threshold_change)
            else:
                a = int(111) # 得出输入到左轮电机值
                b = int(222) # 得出输入到右轮电机值
                c = 'L' + str(a) +'R' + str(b) + 'S' #转化字符串
                c = c.encode("utf-8") #将字符串转化为“utf-8”格式【转化为字节流】
                uart.write(c) #发送字节流
                #print(c)
                '''car.run(0,0)
                String = "1"
                uart.write(String)'''
            while(1):
                if( uart.any() ):
                    time.sleep(50)
                    d1 = uart.read()
                    d2 = str(d1,"utf-8")
                    d3 = eval((d2))
                    #print(d3)
                    if(d3 == 1):
                        threshold_change = green_thresholds
                        break
                    elif(d3 == 3):#爪为空,openmv返回最初状态
                        break
        if(flag == 0):
            a = int(333) # 得出输入到左轮电机值
            b = int(444) # 得出输入到右轮电机值
            c = 'L' + str( a) +'R' + str(b) + 'S' #转化字符串
            c = c.encode("utf-8") #将字符串转化为“utf-8”格式【转化为字节流】
            uart.write(c) #发送字节流
            #print(c)
            '''String = "4"
            uart.write(String)
            for n in range(80000):
                car.run( 0,0 )'''
            while(1):
                if( uart.any() ):
                    time.sleep(50)
                    d4 = uart.read()
                    d5 = str(d4,"utf-8")
                    d6 = eval((d5))
                    #print(d6)
                    if(d6 == 2):
                        threshold_change = red_thresholds #恢复原始颜色阈值
                        action = 1 #恢复原始数值“1”
                        flag = 1 #恢复原始数值“1”
                        flag1=flag1+1
                        if(flag1>=2):
                            flag1=1
                        break

'''(0, 88, 20, 71, -6, 57) 红色阈值

(0, 95, -21, 23, -44, -10) 蓝色阈值

(17, 92, -53, -24, 12, 53) 绿色阈值
'''

'''比赛阈值

(47, 93, -69, -24, 26, 83)  绿色阈值

(5, 99, -25, 24, -41, -12) 蓝色阈值

(41, 83, 24, 83, -18, 60) 红色阈值

'''







