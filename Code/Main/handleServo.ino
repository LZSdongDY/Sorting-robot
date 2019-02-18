//舵机PWM增量处理函数，每隔SERVO_TIME_PERIOD毫秒处理一次，这样就实现了舵机的连续控制
void handleServo() 
{
    static unsigned long systick_ms_bak = 0;
    if(handleTimePeriod(&systick_ms_bak, SERVO_TIME_PERIOD))return;  
    for(byte i = 0; i < SERVO_NUM; i++) 
    {
        if(abs( servo_do[i].aim - servo_do[i].cur) <= abs (servo_do[i].inc) ) 
        {
             myservo[i].writeMicroseconds(servo_do[i].aim);      
             servo_do[i].cur = servo_do[i].aim;
             
        } else 
          {
             servo_do[i].cur +=  servo_do[i].inc;
             myservo[i].writeMicroseconds((int)servo_do[i].cur); 
          }    
    }
}   
