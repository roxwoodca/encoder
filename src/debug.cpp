#ifdef DEBUG_MODE
#include "Arduino.h"
#include "debug.h"

unsigned int num_unread_debugs;
struct debug debug_log[DEBUG_LOG_SIZE];

void log_debug(const char* message, int value, char base=10)
{
  if (num_unread_debugs < DEBUG_LOG_SIZE)
  {
    debug newdebug; 
    strcpy(newdebug.message,message);
    newdebug.value = value;
    newdebug.base = base;
    debug_log[num_unread_debugs++] = newdebug;
  }
}

void print_binary(int v, int num_places)
{
    int mask=0, n;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask;  // truncate v to specified number of places

    while(num_places)
    {

        if (v & (0x0001 << (num_places-1)))
        {
             Serial.print("1");
        }
        else
        {
             Serial.print("0");
        }

        --num_places;
        if(((num_places%4) == 0) && (num_places != 0))
        {
            Serial.print("_");
        }
    }
}

void dump_debugs()
{
  unsigned char i = 0;
  for (i = 0; i < num_unread_debugs; i++)
  {
    Serial.print(debug_log[i].message);
    Serial.print("\t");
    if (debug_log[i].base == 2)
    {
      print_binary(debug_log[i].value,16);
    }
    else
    {
      Serial.print(debug_log[i].value,debug_log[i].base);
    }
    Serial.print("\n");
  }
  num_unread_debugs = 0;

}

void print_hex(int v, int num_places)
{
    int mask=0, n, num_nibbles, digit;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask; // truncate v to specified number of places

    num_nibbles = num_places / 4;
    if ((num_places % 4) != 0)
    {
        ++num_nibbles;
    }

    do
    {
        digit = ((v >> (num_nibbles-1) * 4)) & 0x0f;
        Serial.print(digit, HEX);
    } while(--num_nibbles);

}

void print_float(float f, int num_digits)
{
    int pows_of_ten[4] = {1, 10, 100, 1000};
    int multiplier, whole, fract, d, n;

    multiplier = pows_of_ten[num_digits];
    if (f < 0.0)
    {
        f = -f;
        Serial.print("-");
    }
    whole = (int) f;
    fract = (int) (multiplier * (f - (float)whole));

    Serial.print(whole);
    Serial.print(".");

    for (n=num_digits-1; n>=0; n--) // print each digit with no leading zero suppression
    {
         d = fract / pows_of_ten[n];
         Serial.print(d);
         fract = fract % pows_of_ten[n];
    }
}

#endif
