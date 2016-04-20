#include "encoder.h"

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
  #ifdef DEBUG_MODE
  //log_debug("mux1",mux->value[0],BIN);
  //log_debug("mux2",mux->value[1],BIN);
  #endif
}

// compare encoder values in pairs to their previous value
void read_encoders(encoder_set *enc_set,void (*event_ptr)(int))
{
  // get current mux word
  unsigned char cur_word = enc_set->mux->value[enc_set->mcu_input_pin_index]; 
  unsigned char i, prev_val, cur_val;

  for(i=0; i < enc_set->num_encoders*2; i=i+2)
  {
    prev_val = enc_set->prev_word >> i & 0b11;
    cur_val = cur_word >> i & 0b11; 

    // encoder has been turned clockwise
    if 
        ((prev_val == 0b00 && cur_val == 0b01) ||
	(prev_val == 0b01 && cur_val == 0b11)  ||
	(prev_val == 0b11 && cur_val == 0b10)  ||
        (prev_val == 0b10 && cur_val == 0b00))
    {
       // increment the associated value
       if (enc_set->value[0][i/2] < enc_set->max_value)
       {
         enc_set->value[0][i/2]++;  
       }
       enc_set->cur_encoder = i/2;
    }
    // encoder has been turned anti-clockwise
    else if 
        ((prev_val == 0b00 && cur_val == 0b10) ||
	(prev_val == 0b10 && cur_val == 0b11)  ||
	(prev_val == 0b11 && cur_val == 0b01)  ||
        (prev_val == 0b01 && cur_val == 0b00))
    {
       // decrement the associated value
       if (enc_set->value[0][i/2] > enc_set->min_value)
       {
         enc_set->value[0][i/2]--;  
       }
       enc_set->cur_encoder = i/2;
    }
  } 

  //record the current word for comparison 
  enc_set->prev_word = cur_word; 
}

void read_momentary_switches(momentary_set *mom_set)
{
  // get current mux word
  unsigned char cur_word = mom_set->mux->value[mom_set->mcu_input_pin_index]; 
  unsigned char i, prev_val, cur_val;
  
  for (i= mom_set->bit_offset; i < mom_set->num_switches+mom_set->bit_offset; i++)
  {
    prev_val = mom_set->prev_word >> i & 1;
    cur_val = cur_word >> i & 1;

    if (cur_val > prev_val) 
    {
      log_debug("on",1,DEC);
      (*mom_set->on_switch_down)(i);
    }

    if (cur_val < prev_val)
    {
      //this one is not firing
      log_debug("off",1,DEC);
      (*mom_set->on_switch_up)(i);
    }
  } 
}
