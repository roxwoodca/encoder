#ifndef DEBUG_H
#define DEBUG_H
unsigned int num_unread_debugs = 0;
void log_debug(char message[], int value);
void dump_debugs();

struct debug
{
   unsigned long time;
   char message[16]; 
   int value;
} debug_log[16];

void log_debug(char message[], int value)
{
    debug newdebug; 
    newdebug.time = millis();
    strcpy(newdebug.message,message);
    newdebug.value = value;
    //debug newdebug = { millis(), message, value }; 
    debug_log[num_unread_debugs++] = newdebug;
}

void dump_debugs()
{
  Serial.begin(9600);
  unsigned int i = 0;
  for (i = 0; i < num_unread_debugs; i++)
  {
    Serial.print(debug_log[i].time);
    Serial.print("\t");
    Serial.print(debug_log[i].message);
    Serial.print("\t");
    Serial.print(debug_log[i].value);
  }
  num_unread_debugs = 0;
}
#endif /* DEBUG_H */
