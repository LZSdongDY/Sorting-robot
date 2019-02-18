unsigned char handleActionGroup(const char (*ActionGroup)[CHAR_NUM], unsigned int AG_NUM ) //动作组的执行
{
  static unsigned long systick_ms_bak = 0, A = 0;
  if (handleTimePeriod(&systick_ms_bak, time_max)) return 1;
  time_max = 10;
  strcpy_P(buffer, ActionGroup[A]); // Necessary casts and dereferencing, just copy.
  inString = buffer;
  A++;
  if (A == AG_NUM)
  {
    A = 0;
    return A;
  }
  else
  {
    return A;
  }
}
