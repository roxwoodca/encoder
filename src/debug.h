unsigned int num_unread_debugs = 0;

struct debug
{
   unsigned long time;
   char message[8]; 
   int  value;
} debug_log[16]

void log_debug(char message, value)
{
    debug newdebug = { millis(), message, value }; 
    debug_log[num_unread_debugs++] = newdebug;
}

void dump_debugs()
{
  int i = 0;
  for (i = 0; i < debug_log_size; i++)
  {
    Serial.print(debug_log[i].time);
    Serial.print("\t");
    Serial.print(debug_log[i].message);
    Serial.print("\t");
    Serial.print(value);
  }
  num_unread_debugs = 0;
}
