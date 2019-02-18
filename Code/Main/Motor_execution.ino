void Motor_execution() //电机执行函数
{
  Speed_1 = uartReceive_1(); //接收串口信息，将串口信息转换为控制电机的变量并存储到速度数组中，返回数组指针
     
     if( (Speed_1[0] == 666) && (logol_3 == 1) )
     {
        car_run(190,190);
        delay(300);
        car_run(0,0);
        logol_3 = 2;  
        Speed_1[0] = 0;
        Speed_1[1] = 0;
        ActionGroupNumber = 2;
      }
      
     if( (Speed_1[0] == 111) && (logol_3 == 1) )
     {
        car_run(0,0);
        logol_3 = 2;  
        Speed_1[0] = 0;
        Speed_1[1] = 0;
        ActionGroupNumber = 2;
      }
      
     if( ((Speed_1[0] < -255) || (Speed_1[0] > 255)) && (logol_3 == 2) )
     {
        car_run(0,0);
        logol_3 = 1;
        Speed_1[0] = 0; 
        Speed_1[1] = 0;
        ActionGroupNumber = 3;
      }
     
     car_run(Speed_1[0],Speed_1[1]); //电机驱动函数
  }
