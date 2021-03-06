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
  signed int value[4][4];
  unsigned char mcu_input_pin_index;
  unsigned char num_encoders;
  unsigned char min_value;
  unsigned char max_value;
  unsigned char cur_encoder;  
  unsigned char cur_bank;
};

struct momentary_set
{
  struct digital_mux *mux;
  unsigned char prev_word;
  unsigned char mcu_input_pin_index;
  unsigned char num_switches;
  void (*on_switch_down)(char);
  void (*on_switch_up)(char);
  void (*on_single_click)(char);
  void (*on_double_click)(char);
  void (*on_long_closed)(char);
  unsigned char bit_offset;
  unsigned expiry_time;
  struct momentary_session *session;
};

struct momentary_session
{
  unsigned char status[4];
  unsigned int  expiry[4];
};

// function prototypes
void scan_mux(digital_mux *mux);
void read_encoders(encoder_set *twddle_enc,void (*event_ptr)(int));
void read_momentary_switches(momentary_set *twddle_mom);
void set_cur_bank(encoder_set *enc_set, char index);

#endif /* MCUIO_H */
