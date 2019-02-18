void Placing_balls() //执行放置小球到存储区动作
{
          while(over) //执行放置小球到存储区动作
          {
              over = handleActionGroup(ActionGroup1,AG_NUM1);  //获取动作组
              parseInStringCmd();                             //解析动作组
              handleServo();                                  //执行动作组
            }
          over = 1;
          
          car_run(190,190);
          while(1)
          {  
            if( Ultrasonic() < 10 || p == 400 )
              {
                  p = 0; 
                  while(over) //执行放置小球到存储区动作
                  {
                    over = handleActionGroup(ActionGroup1_1,AG_NUM1_1);  //获取动作组
                    parseInStringCmd();                             //解析动作组
                    handleServo();                                  //执行动作组
                  }
                  over = 1;
          
                  car_run(0,0);
                  while(over) //执行放置小球到存储区动作【爪子张开】
                  {
                    over = handleActionGroup(ActionGroup1_1,AG_NUM1_1);  //获取动作组
                    parseInStringCmd();                                  //解析动作组
                    handleServo();                                       //执行动作组
                  }
                  over = 1;
          
                  car_run(-190,-190);
                  while(over) //保持某个姿态寻找小球
                  {
                    over = handleActionGroup(ActionGroup2_1,AG_NUM2);  //获取动作组
                    parseInStringCmd();                             //解析动作组
                    handleServo();                                  //执行动作组
                  }
                  over = 1;
          
                  car_run(0,0);
                  ActionGroupNumber = 6;
                  //String logol_2= "2";
                  Serial.println(logol_2); //发送"2"标志         
                  break;
                }
            p+=1;
          }
  }
