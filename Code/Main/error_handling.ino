void Ultrasonic_error_handling() //超声波工作异常指示
{
   for(unsigned char i;i<10;i++) //以特定的频率闪烁
   {
      digitalWrite(LED,HIGH);
      delay(50);
      digitalWrite(LED,LOW);
      delay(50);
    }
  }
