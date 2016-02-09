#ifndef DEBUG_H
#define DEBUG_H
unsigned int num_unread_debugs = 0;
void log_debug(char message[], int value);
void dump_debugs();

struct debug
{
   char message[8]; 
   int value;
} debug_log[8];

void log_debug(const char* message, int value)
{
  if (num_unread_debugs < 8)
  {
    debug newdebug; 
    strcpy(newdebug.message,message);
    newdebug.value = value;
    debug_log[num_unread_debugs++] = newdebug;
  }
}

void dump_debugs()
{
  unsigned char i = 0;
  for (i = 0; i < num_unread_debugs; i++)
  {
    Serial.print(debug_log[i].message);
    Serial.print("\t");
    Serial.print(debug_log[i].value);
    Serial.print("\n");
  }
  //Serial.print("\n");
  num_unread_debugs = 0;

}
#endif /* DEBUG_H */
