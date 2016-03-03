#ifndef NUMDISP_H
#define NUMDISP_H

#ifdef DEBUG_MODE
#include "Arduino.h"
#include "debug.h"
#endif

struct numeric_display
{
  unsigned char num_digits;
  volatile unsigned char* ctr_port;
  unsigned char ctr_pin_offset;
  unsigned char num_ctr_bits;
  volatile unsigned char* strb_port;
  unsigned char strb_pin_offset;
  unsigned char cur_digit;
  unsigned int value;
  unsigned int expiry_time;
  unsigned int cur_expiry; 
};


// function prototypes
void num_disp_write(int val, numeric_display *disp);
void num_disp_write_cur_digit(numeric_display *disp);
void num_disp_enable_cur_digit(numeric_display *disp);
char get_decimal_column(unsigned int val, char digit_offset);

#endif /* NUMDISP_H */
