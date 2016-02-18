#ifndef MCUIO_H
#define MCUIO_H

#define NUM_DIGITAL_INS  2
#define NUM_DIGITAL_OUTS 3

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
  signed char value[8];
  unsigned char mcu_input_pin_index;
  unsigned char num_encoders;
  unsigned char cur_expiry[8];
  unsigned char expiry_life;
};

// function prototypes
void scan_mux(digital_mux *mux);
void process_encoder_data(encoder_set *twddle_enc);

#endif /* MCUIO_H */