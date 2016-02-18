#include "Arduino.h"
#include "interrupts.h"
#include "encoder.h"

#ifdef DEBUG_MODE
#include "debug.h"
#endif

digital_mux twddle_mux = { 8, &PORTB, 1, 3, &PORTB, 5, 2, { 0, 0 } };
encoder_set twddle_enc = { &twddle_mux, 0, { 0 }, 1, 4, { 16 }, 16 };

// the setup routine runs once when you press reset:
void setup() 
{
  #ifdef DEBUG_MODE
  Serial.begin(9600);
  #endif

  // set I/O direction register
  DDRB =0x0E;

  init_interrupts_uno();
}

/* 2KHz */
void isr_0()
{
  scan_mux(&twddle_mux);
  read_encoders(&twddle_enc);
}

/* 1Hz */
void isr_1()
{
  #ifdef DEBUG_MODE
  dump_debugs();
  #endif
}

void loop() { }

/* This function steps through the channels of each mux (using the shared channel selector)
 * shifting each bit to the left to eventually read a full byte 
 */
void scan_mux(digital_mux *mux)
{
  unsigned int i, j, bit_mask, zero_mask, cur_bit;

  // loop through mux out channels in parallel 
  for(i=0; i < mux->num_channels; i++)
  {
    // zero out selector out pins 
    zero_mask = 0 | ~(((1 << mux->num_selector_pin)-1) << mux->selector_pin_offset);
    *mux->selector_port &= zero_mask;

    // write current channel number into selector 
    bit_mask = (i << mux->selector_pin_offset); 
    *mux->selector_port |= bit_mask;

    // loop through input ports, shifting to get value of each pin
    //  and shifting them onto a byte for each input 
    for (j=0; j < mux->num_input_pin; j++)
    {
      cur_bit = ((*mux->input_port >> (mux->input_pin_offset+j)) & 0x01);
      mux->value[j] = ((mux->value[j]<<1) | cur_bit);
    }
  }
}

// compare encoder values in pairs to their previous value
void read_encoders(encoder_set *enc_set)
{
  // get current mux word
  unsigned char cur_word = enc_set->mux->value[enc_set->mcu_input_pin_index]; 
  unsigned char i, prev_val, cur_val;

  for(i=0; i < enc_set->num_encoders*2; i=i+2)
  {
    prev_val = enc_set->prev_word >> i & 0b11;
    cur_val = cur_word >> i & 0b11; 

    //if no values have changed or the previous value stored has not yet expired
    if (cur_val == prev_val || enc_set->cur_expiry[i/2] < enc_set->expiry_life)
    {
      //decrement the expiry
      if (enc_set->cur_expiry > 0)
      {
        enc_set->cur_expiry[i/2]--;
      }
      //unless it is at 0 already, in which case reset the expiry counter
      else
      { 
        enc_set->cur_expiry[i/2] = enc_set->expiry_life;
      }
    } 
    // encoder has been turned clockwise
    else if 
        ((prev_val == 0b00 && cur_val == 0b01) ||
	(prev_val == 0b01 && cur_val == 0b11)  ||
	(prev_val == 0b11 && cur_val == 0b10)  ||
        (prev_val == 0b10 && cur_val == 0b00))
    {
       // increment the associated value
       log_debug("cw",i);
       enc_set->value[i/2]++;  
    }
    // encoder has been turned anti-clockwise
    else if 
        ((prev_val == 0b00 && cur_val == 0b10) ||
	(prev_val == 0b10 && cur_val == 0b11)  ||
	(prev_val == 0b11 && cur_val == 0b01)  ||
        (prev_val == 0b01 && cur_val == 0b00))
    {
       // decrement the associated value
       log_debug("ccw",i);
       enc_set->value[i/2]--;  
    }
  } 

  //record the current word for comparison 
  enc_set->prev_word = cur_word; 
}
