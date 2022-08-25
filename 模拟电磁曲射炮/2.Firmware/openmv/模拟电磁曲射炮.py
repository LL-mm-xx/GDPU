#提示包未安装的自己找教程安装一下-_-
import sensor, image, time, math
from pyb import UART
import json
import ustruct

sensor.reset()
sensor.set_framesize(sensor.QVGA)
sensor.set_pixformat(sensor.RGB565)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)     #关掉白平衡和自动增益
#sensor.set_auto_gain(False) # must be turned off for color tracking
#sensor.set_auto_whitebal(False) # must be turned off for color tracking
#sensor.set_auto_exposure(False,2000);  #控制曝光时间,单位为us
#sensor.set_hmirror(True); #控制水平镜像翻转
#sensor.set_vflip(True); #控制水平镜像翻转
#sensor.set_windowing((22,7,110,105));

clock = time.clock()
uart = UART(3,115200)   #定义串口3变量    P4 TX<-->PA10  P5 RX<-->PA9
uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters

#binary_threshold = (0, 156)
find_threshold = (30, 100, 25, 127, -128, 127)
K = 11240;  #自己选取一个合适的校准值
def find_max(blobs):    #定义寻找色块面积最大的函数
    blobs.sort(key=lambda x:x.pixels(),reverse=True);
    max_blob={}             #默认为空字典
    length=len(blobs);
    if length>0:
        max_blob=blobs[0];
    return max_blob;

def sending_data(cx_max,cy_max):
    global uart;
    #frame=[0x2C,18,cx%0xff,int(cx/0xff),cy%0xff,int(cy/0xff),0x5B];
    #data = bytearray(frame)
    data = ustruct.pack("<bbhhb",              #格式为俩个字符俩个短整型(2字节)
                   0x2C,                       #帧头1
                   0x12,                       #帧头2
                   int(cx_max), # up sample by 4    #数据1
                   int(cy_max), # up sample by 4    #数据2LCD_ShowStringLCD_ShowString
                   0x5B);
    uart.write(data);   #必须要传入一个字节数组

while(True):
    clock.tick()
    img = sensor.snapshot()#.lens_corr(1.45);
    #img.binary([binary_threshold], invert = 1)
    blobs = img.find_blobs([find_threshold],area_threshold=150)

    if blobs:	#如果找到的话
        max_blob=find_max(blobs)
        img.draw_rectangle(max_blob.rect(),color=(0,0,255))
        img.draw_cross(max_blob.cx(), max_blob.cy(),color=(0,0,255))
        img.draw_cross(160, 120,color=(0,0,255)) # 在中心点画标记
        img.draw_line((160,120,max_blob.cx(),max_blob.cy()), color=(0,0,255));

        phi = (max_blob.w() + max_blob.h())/2;
        length = K/phi; #获得距标靶距离

        #print('position:',max_blob.cx(),max_blob.cy())
        if(max_blob.pixels()>1000):	#滤除像素过小的干扰
            sending_data(max_blob.cx(),length); #发送点位坐标
            #print(max_blob.pixels())
            print("Length=",length);
    else:
        sending_data(10000,10000);
    #print(clock.fps())
