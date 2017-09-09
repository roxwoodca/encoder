#ifndef DEBUG_H
#define DEBUG_H
#define DEBUG_LOG_SIZE 24

void dump_debugs();
void print_binary(int, int);
void log_debug(const char*, int, char);

struct debug
{
   char message[8]; 
   unsigned int  value;
   char base;
};
//}extern debug_log[DEBUG_LOG_SIZE];

#endif /* DEBUG_H */
