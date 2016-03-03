#include "num_disp.h"

/*
 * write an integer value to a 7 segment display of x digits 
 * using strobed mux method
 */

void num_disp_write(int val, numeric_display *disp)
{
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
  // pass pointer to 7 seg struct containing state values
  num_disp_write_cur_digit(disp);

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

void num_disp_write_cur_digit(numeric_display *disp)
{
  // zero out counter pins 
  unsigned char zero_mask = 0 | ~(((1 << disp->num_ctr_bits)-1) << disp->ctr_pin_offset);
  *disp->ctr_port &= zero_mask;

  // extract the digit value
  unsigned char digit = get_decimal_column(disp->value,disp->cur_digit); 

  #ifdef DEBUG_MODE
  log_debug("val",disp->value,10);
  log_debug("dig_indx",disp->cur_digit,10);
  log_debug("digit",digit,10); 
  #endif

  // create a mask to switch on the right pins
  unsigned char bit_mask = (digit << disp->ctr_pin_offset);
  *disp->ctr_port |= bit_mask;
}

void num_disp_enable_cur_digit(numeric_display *disp)
{
  // zero out strobe pins 
  unsigned char zero_mask = 0 | ~(((1 << disp->num_digits)-1) << disp->strb_pin_offset);
  *disp->strb_port &= zero_mask;
 
  // create a mask to switch on the required digit
  unsigned char bit_mask = 1 << (disp->strb_pin_offset + disp->cur_digit);
  *disp->strb_port |= bit_mask;
}

char get_decimal_column(unsigned int val, char digit_offset)
{
  /* example: val is 1234
   * offset val is 2 so we want 100s
   * so move the decimal point 2 jumps to the left
   * 1234 / 10^2 = 12 (everything after dp is discarded)
   * 12 % 10 = 2 
   * returns 2
   */

  return (val / (10^digit_offset)) % 10;
}
