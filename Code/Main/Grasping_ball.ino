void Grasping_ball() //执行抓取小球动作
{
          while(over) //执行抓取小球动作
          {
              over = handleActionGroup(ActionGroup6,AG_NUM6);  //获取动作组
              parseInStringCmd();                             //解析动作组
              handleServo();                                  //执行动作组
            }
          over = 1;
          
          car_run(190,-190);
          delay(100);
          car_run(190,190);
          delay(500);
          car_run(0,0);
          while(over) //执行抓取小球动作
          {
              over = handleActionGroup(ActionGroup7,AG_NUM7);  //获取动作组
              parseInStringCmd();                             //解析动作组
              handleServo();                                  //执行动作组
            }
          over = 1;
          
          if(digitalRead(check) == 0)
          {
            while(over) //保持某个姿态寻找存储区
            {
              over = handleActionGroup(ActionGroup_,AG_NUM_);  //获取动作组
              parseInStringCmd();                             //解析动作组
              handleServo();                                  //执行动作组
            }
            over = 1;
            
            ActionGroupNumber = 6;
            //String logol_1= "1";
            Serial.println(logol_1); //发送"1"标志
          }
          else
          {
              logol_3 = 1; //logol_3标识符恢复原始状态值
              while(over) //保持某个姿态寻找小球的动作
              {
              over = handleActionGroup(ActionGroup2,AG_NUM2);  //获取动作组
              parseInStringCmd();                             //解析动作组
              handleServo();                                  //执行动作组
              }
              over = 1;
              
              ActionGroupNumber = 6;
              count_1++;
              if(count_1 == 3)
              {
                  count_1 = 0;
                  car_run(-180,-180);
                  delay(500);
                  car_run(190,-190);
                  delay(500);
                  car_run(0,0);
                }
              //String back = "3"; 
              Serial.println(back); //发送openmv返回找小球状态的标识符
            }
  }
