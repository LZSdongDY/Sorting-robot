/*
时间处理函数，第一个参数是上一次处理时间点， 第二个参数是处理时间间隔，
到达时间间隔返回0，否则返回1
*/
bool handleTimePeriod( unsigned long *ptr_time, unsigned int time_period) 
{
    if((millis() - *ptr_time) < time_period) 
    {
        return 1;  
    } else
    {
         *ptr_time = millis();
         return 0;
    }
}
