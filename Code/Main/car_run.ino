void car_run(int left_speed1,int right_speed1)
{
    if(inverse_left == 0)
    {
        left_speed1 = (-left_speed1);
      }
    if(inverse_right == 0)
    {
        right_speed1 = (-right_speed1);
      } 
    if(left_speed1 < 0)
    {
        digitalWrite(AIN1,LOW);
        digitalWrite(AIN2,HIGH);
      }
    else
    {
        digitalWrite(AIN1,HIGH);
        digitalWrite(AIN2,LOW);
      }
    analogWrite(PWMA,abs(left_speed1));

    if(right_speed1 < 0)
    {
        digitalWrite(BIN1,LOW);
        digitalWrite(BIN2,HIGH);
      }
    else
    {
        digitalWrite(BIN1,HIGH);
        digitalWrite(BIN2,LOW);
      }
    analogWrite(PWMB,abs(right_speed1));
  }
