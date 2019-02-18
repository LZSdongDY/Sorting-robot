#include <Servo.h>                 //声明调用Servo.h库
#define  SERVO_NUM  6              //宏定义舵机个数
#define  SERVO_TIME_PERIOD  20     //每隔20ms处理一次（累加）舵机的PWM增量 
#define AG_NUM_    1                //抓取小球动作组数
#define AG_NUM1    1                //放置小球动作组数
#define AG_NUM1_1  1                //放置小球动作组数
#define AG_NUM2    1                //保持某个姿态寻找小球的动作组数
#define AG_NUM3    2                //保持某个姿态寻找存储区的动作组数
#define AG_NUM4    1                //初始化状态动作组数
#define AG_NUM5    2                //初始化状态动作组数
#define AG_NUM6    1                //抓取小球动作组（新1）
#define AG_NUM7    1                //抓取小球动作组（新2）
#define CHAR_NUM  100              //每个动作组最大字节数，每个舵机指令字符15个
String logol_1 = "1";              //触发寻找绿色区域的标识符
String logol_2 = "2";              //触发寻找小球的标识符
String back = "3";                 //openmv返回找小球状态标识符
String inString = ""; //初始化字符串
unsigned char logol_3 = 1;         //触发执行动作组的标识符
unsigned char ActionGroupNumber = 1; //执行动作组标识符
unsigned int p = 0; //判断爪子是否要张开标识符
unsigned int time_max = 0;

//舵机驱动
byte  servo_pin[SERVO_NUM] = {10, A2, A3, A0, A1 ,7};            //宏定义舵机控制引脚
Servo myservo[SERVO_NUM];         //创建一个舵机类
const char ActionGroup_[AG_NUM_][CHAR_NUM] PROGMEM = {//存储保持某个姿态寻找存储区动作组
      "{G0026#000P1500T0100!#001P1300T0300!#002P2300T0300!#003P1800T0300!#004P2150T0300!#005P1500T0300!}", 
};
      
const char ActionGroup1[AG_NUM1][CHAR_NUM] PROGMEM = {//存储放置小球到存储区动作组 
     "{G0022#000P1500T0050!#001P1250T0050!#002P1620T0050!#003P1700T0050!#004P0880T0050!#005P1500T0050!}",
};

const char ActionGroup1_1[AG_NUM1_1][CHAR_NUM] PROGMEM = {//存储放置小球到存储区动作组
    "{G0023#000P1500T0050!#001P1250T0050!#002P1620T0050!#003P1700T0050!#004P1500T0050!#005P1500T0050!}",
};

const char ActionGroup2[AG_NUM2][CHAR_NUM] PROGMEM = {//保持某个姿态寻找小球的动作组  
    "{G0031#000P1500T0100!#001P1150T0100!#002P2300T0100!#003P1650T0100!#004P2150T0100!#005P1520T0100!}",
};

const char ActionGroup2_1[AG_NUM2][CHAR_NUM] PROGMEM = {//保持某个姿态寻找小球的动作组 
    "{G0031#000P1500T0800!#001P1150T0800!#002P2300T0800!#003P1650T0800!#004P2150T0800!#005P1520T0800!}",
};

const char ActionGroup3[AG_NUM3][CHAR_NUM] PROGMEM = {//旋转机械爪  
    "{G0024#000P1500T0100!#001P1250T0100!#002P1620T0100!#003P1700T0100!#004P1500T0100!#005P2000T0100!}",
    "{G0025#000P1500T0100!#001P1400T0500!#002P1620T0500!#003P1600T0500!#004P1800T0500!#005P2000T0500!}",
};

const char ActionGroup4[AG_NUM4][CHAR_NUM] PROGMEM = {//初始化状态动作组 
    "{G0004#000P1500T0500!#001P1250T1000!#002P1900T1000!#003P2000T1000!#004P1300T1000!#005P1500T1000!}",
};

const char ActionGroup5[AG_NUM5][CHAR_NUM] PROGMEM = {//初始化状态动作组 
    "{G0005#000P1500T0500!#001P1200T0200!#002P1900T1000!#003P2000T1000!#004P1800T0100!#005P1500T1000!}",
    "{G0006#000P1500T1000!#001P1150T0500!#002P2300T0500!#003P1650T0500!#004P2150T0500!#005P1520T0600!}",
};

const char ActionGroup6[AG_NUM6][CHAR_NUM] PROGMEM = {//抓取小球动作组（新1） 
    "{G0032#000P1500T1500!#001P1020T0100!#002P2050T0100!#003P1750T0100!#004P2100T0100!#005P1850T0050!}",
};

const char ActionGroup7[AG_NUM7][CHAR_NUM] PROGMEM = {//抓取小球动作组（新2）
    "{G0034#000P1500T1500!#001P1000T0100!#002P2000T0100!#003P1750T0100!#004P2100T0100!#005P1520T0050!}",
};

char buffer[100];    // 定义一个数组用来存储每小组动作组
typedef struct {                  //舵机结构体变量声明
    unsigned int aim = 1500;      //舵机目标值 
    float cur = 1500.0;           //舵机当前值
    unsigned  int time1 = 1000;   //舵机执行时间
    float inc= 0.0;               //舵机值增量，以20ms为周期
}duoji_struct;
duoji_struct servo_do[SERVO_NUM]; //用结构体变量声明一个舵机变量组

unsigned char over=1; //动作组是否执行完毕标识符

//电机驱动
String inString_1 = ""; //初始化字符串
unsigned char len,h; //定义字符串长度变量
unsigned char logol_q,logol_w; //判断标识符
int left_speed,right_speed;  //定义左右轮速度的变量
int Speed[2] = {0,0}; //定义一个存放速度值的数组
int *Speed_1; //定义一个指向存放速度值的数组的指针

unsigned char inverse_left = 0; //将其改为“1”使电机反转
unsigned char inverse_right = 0; //将其改为“1”使电机反转

unsigned char AIN1 = 2; //定义电机驱动板引脚
unsigned char AIN2 = 4; //定义电机驱动板引脚
unsigned char PWMA = 3; //定义电机驱动板引脚

unsigned char BIN1 = 8; //定义电机驱动板引脚
unsigned char BIN2 = 9; //定义电机驱动板引脚
unsigned char PWMB = 5; //定义电机驱动板引脚

//超声波模块驱动
#define trigPin 6 //定义超声波模块trig引脚
#define echoPin 11 //定义超声波模块echo引脚
unsigned long distance; //距离变量

//避障传感器模块驱动
#define check A4 //定义避障信号引脚

//抓取不了小球的次数累计
unsigned char count_1 = 0; 

//传感器错误处理指示灯
#define LED 13

     
//初始化函数                    
void setup()
{ 
    Serial.begin(115200);  //初始化波特率为115200
  
    pinMode(PWMA, OUTPUT); //初始化电机驱动模块引脚
    pinMode(AIN1, OUTPUT); //初始化电机驱动模块引脚
    pinMode(AIN2, OUTPUT); //初始化电机驱动模块引脚
 
    pinMode(PWMB, OUTPUT); //初始化电机驱动模块引脚
    pinMode(BIN1, OUTPUT); //初始化电机驱动模块引脚
    pinMode(BIN2, OUTPUT); //初始化电机驱动模块引脚
    
    for(byte i = 0; i < SERVO_NUM; i++) //将引脚与声明的舵机对象连接起来
    {
        myservo[i].attach(servo_pin[i]);   
    }

    pinMode(trigPin,OUTPUT); //初始化超声波引脚
    pinMode(echoPin,INPUT);  //初始化超声波引脚

    pinMode(check,INPUT); //初始化避障传感器

    pinMode(LED,OUTPUT); //初始化指示灯引脚
} 

//主函数
void loop()
{    
     switch(ActionGroupNumber) //ActionGroupNumber初始值为“1”
     {
        case 1:
          Find_ball(); //寻找小球
          break;
            
        case 2:
          Grasping_ball(); //抓取小球
          break;

        case 3:
          Placing_balls(); //放置小球
          break;
        
        default: 
          break;
      } 
     Motor_execution(); //电机执行
  }


