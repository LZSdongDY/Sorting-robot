//接收串口发来的字符串
int * uartReceive_1(void){                //串口接收函数，单个字节接收   
    while (Serial.available()>0) {   //如果串口有数据
        char inChar = char(Serial.read()); //读取串口字符
        inString_1.concat(inChar); //(concatenate) 连接
        delay(1);//为了防止数据丢失,在此设置短暂延时delay(2)
    }
    if (inString_1.length() > 0){ //判断是否有字符串数据
        if(inString_1[0] == 'L')  //判断是否为起始符“L”
        {
            len = inString_1.length(); //获取串口接收数据的长度
            left_speed = 0;right_speed = 0; //初始化参数
            for(h = 0; h < len; h++)
            {
                if(inString_1[h] == 'L') //判断是否为起始符“L”
                {
                    h++;  //下个字符
                    if(inString_1[h] == '-') //判断接受字符是否为“-”
                    {
                        logol_q = 1;
                        h++;
                      }
                    while((inString_1[h] != 'R') && (h<len)) //判断是否为‘R’之前的字符
                    {
                        left_speed = left_speed * 10 + (inString_1[h] - '0');
                        h++;
                      }
                      h--;
                  } else if(inString_1[h] == 'R')
                  {
                      h++;
                      if(inString_1[h] == '-')
                      {
                          logol_w = 1;
                          h++;
                        }
                      while((inString_1[h] != 'S') && (h < len))
                      {
                          right_speed = right_speed * 10 + (inString_1[h] - '0');
                          h++; 
                        }
                    }
                    if(logol_q == 1)
                    {
                        logol_q = 0;
                        left_speed = -left_speed;
                      }
                    if(logol_w == 1)
                    {
                        logol_w = 0;
                        right_speed = -right_speed;
                      } 
              }
          }
          //left_speed = constrain(int(left_speed),-255,255);
          //right_speed = constrain(int(right_speed),-255,255);
          Speed[0] = left_speed;
          Speed[1] = right_speed;
          inString_1 = "";
          return Speed;
    }                
}
