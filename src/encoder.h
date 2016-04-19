#ifndef ENCODER_H
#define ENCODER_H

#ifdef DEBUG_MODE
#include "Arduino.h"
#include "debug.h"
#endif

// # of mux outputs is hardcoded to 2. need to find a way to make this flexible
// without taking up unnecessary sdram
struct digital_mux
{
  unsigned char num_channels; 
  volatile unsigned char* selector_port; 
  unsigned char selector_pin_offset;
  unsigned char num_selector_pin;
  volatile unsigned char* input_port; 
  unsigned char input_pin_offset;
  unsigned char num_input_pin;
  unsigned char value[2];
};

// same issue as above for value[] and num_encoders
struct encoder_set
{
  struct digital_mux *mux;
  unsigned char prev_word;
  signed int value[8];
  unsigned char mcu_input_pin_index;
  unsigned char num_encoders;
  unsigned char min_value;
  unsigned char max_value;
  unsigned char cur_encoder;  
};

// function prototypes
void scan_mux(digital_mux *mux);
void read_encoders(encoder_set *twddle_enc,void (*event_ptr)(int));

#endif /* MCUIO_H */
