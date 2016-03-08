#include "num_disp.h"

/*
 * write an integer value to a 7 segment display of x digits 
 * using strobed mux method
 */

void num_disp_write(int val, numeric_display *disp)
{
  // convert the value to individual digits
  unsigned char *bcd;
  bcd = to_bcd(val); 


  // decrement the expiry timer and exit if it has a positive value
  // this is the period during which a digit displays
  if (disp->cur_expiry > 0)
  {
    disp->cur_expiry--;
    return;
  }

  // if we are back to the ones column then refresh value with the latest value
  if (disp->cur_digit==0)
  {
    disp->value = val;
  }

  // write the specified digit
  // pass pointer to 7 seg struct, and char bcd digit val 
  num_disp_write_digit(disp, *(bcd+disp->cur_digit));

  // turn on the specified digit (all digits are off by default)
  num_disp_enable_cur_digit(disp);
 
  //set the expiry timer so the digit can display for a while 
  disp->cur_expiry = disp->expiry_time;
 
  // if there are still digits to go 
  // then increment to next column and reset the expiry
  if ((disp->cur_digit+1) < disp->num_digits)
  { 
    disp->cur_digit++;
  }
  // or reset to 1s and restart the expiry timer
  else
  {
    disp->cur_digit=0;
  }

}

void num_disp_write_digit(numeric_display *disp, unsigned char digit)
{
  // zero out counter pins 
  unsigned char zero_mask = 0 | ~(((1 << disp->num_ctr_bits)-1) << disp->ctr_pin_offset);
  *disp->ctr_port &= zero_mask;

  #ifdef DEBUG_MODE
  log_debug("val",disp->value,10);
  log_debug("dig_indx",disp->cur_digit,10);
  log_debug("digit",digit,10); 
  #endif

  // create a mask to switch on the right pins
  unsigned char bit_mask = (digit << disp->ctr_pin_offset);
  //log_debug("mask",bit_mask,BIN); 
  *disp->ctr_port |= bit_mask;
}

void num_disp_enable_cur_digit(numeric_display *disp)
{
  //disp->cur_digit=0;
  // disable all displays
  unsigned char reset_mask = 0 | (((1 << disp->num_digits)-1) << disp->strb_pin_offset);
  *disp->strb_port |= reset_mask;
 
  // create a mask to enable the current display (with a low signal)
  unsigned char bit_mask = ~(1 << (disp->strb_pin_offset + disp->cur_digit));
  //log_debug("mask",bit_mask, BIN);
  *disp->strb_port &= bit_mask;
}


unsigned char * to_bcd(unsigned char input)
{
  static unsigned char digit[3]; 
  // divide binary input by 10
  // e.g. 123/10=12
  // save the intermediate result
  // 12->[D1]
  digit[1] = input / 10; 

  // multiply the result by 10
  // 12*10=120
  // subtract the multiplication result from binary input number
  // 123-120=3
  // store the result in [units] 
  // 3->[D0]
  digit[0] = input-(digit[1]*10);

  // divide the intermediate redult by 10
  // 12/10=1
  // store the result in [hundreds]
  // 1->[D2]
  digit[2] = digit[1]/10;

  // multiply the hundreds by 10
  // 1*10=10
  // subtract the multiplication result from the intermediate result
  // 12-10=2
  // store the result in [tens]
  digit[1] = digit[1]-(digit[2]*10);
  
  return digit;
}

long int x_to_the_n (int x,int n)
{
    int i;
    int val = 1;

    for (i = 0; i < n; ++i)
    {
      val *= x;
    }

    return val;
}
