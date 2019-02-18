//解析串口接收指令   
void parseInStringCmd(){
 static unsigned int index, time1, pwm1, i, len;//声明三个变量分别用来存储解析后的舵机序号，舵机执行时间，舵机PWM
    static int left_pwm, right_pwm;
    static bool flag=0;
    if(inString.length() > 0)  //判断串口有数据  
    {       
        if((inString[0] == '#') || (inString[0] == '{'))  //解析以“#”或者以“{”开头的指令
        {   
            len = inString.length();       //获取串口接收数据的长度
            index=0; pwm1=0; time1=0;           //3个参数初始化
            for(i = 0; i < len; i++) 
            {         
                if(inString[i] == '#') //判断是否为起始符“#”
                {        
                    i++;                        //下一个字符
                    while((inString[i] != 'P') && (i<len)) //判断是否为#之后P之前的数字字符
                    {     
                        index = index*10 + (inString[i] - '0');  //记录P之前的数字
                        i++;
                    }
                    i--;                          //因为上面i多自增一次，所以要减去1个
                } else if(inString[i] == 'P') //检测是否为“P”
                {   
                    i++;
                    while((inString[i] != 'T') && (i<len)) //检测P之后T之前的数字字符并保存
                    {  
                        pwm1 = pwm1*10 + (inString[i] - '0');
                        i++;
                    }
                    i--;
                } else if(inString[i] == 'T') //判断是否为“T”
                {  
                    i++;
                    while((inString[i] != '!') && (i<len))  //检测T之后!之前的数字字符并保存
                    {
                        time1 = time1*10 + (inString[i] - '0'); //将T后面的数字保存
                        i++;
                    }
                    if((index >= SERVO_NUM) || (pwm1 > 2500) ||(pwm1 < 500)|| (time1>10000)) //如果舵机号和PWM数值超出约定值则跳出不处理 
                    {  
                        break;
                    }
                    if(time1<SERVO_TIME_PERIOD)time1=SERVO_TIME_PERIOD;//很重要，防止被除数为0
                    //检测完后赋值
                    if(time_max<time1)time_max=time1;
                    servo_do[index].aim = pwm1;         //舵机PWM赋值
                    servo_do[index].time1 = time1;      //舵机执行时间赋值
                    float pwm_err = servo_do[index].aim - servo_do[index].cur;
                    servo_do[index].inc = (pwm_err*1.00)/(servo_do[index].time1/SERVO_TIME_PERIOD); //根据时间计算舵机PWM增量   
                    index = pwm1 = time1 = 0; 
                }
            }
        }                                                                      
    }
     
    inString = ""; 
    //inString = "";//放到解析外面，每次都清除接收信息，保障每次解析的都是最新数据*****
}
