unsigned long Ultrasonic() //超声波测距函数
{
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    distance=pulseIn(echoPin,HIGH)/58.00;
    delay(10);
    if(distance<0 && distance ==0)
    {
        Ultrasonic_error_handling(); //超声波工作异常提示
      }
    return distance;
  }
