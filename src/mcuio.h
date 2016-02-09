#ifndef MCUIO_H
#define MCUIO_H

#define NUM_DIGITAL_INS (sizeof digital_ins / sizeof digital_ins[0])
#define NUM_DIGITAL_OUTS (sizeof digital_outs / sizeof digital_outs[0])
#define NUM_ANALOG_INS (sizeof analog_ins / sizeof analog_ins[0])
#define NUM_ANALOG_OUTS (sizeof analog_outs / sizeof analog_outs[0])

// structs
struct digital_in
{
  const char pin;
  volatile unsigned char value;
};

struct digital_out
{
  const char pin;
  volatile unsigned char value;
};

struct analog_in
{
  const unsigned char pin;
  const unsigned char resolution;
  int sensor_min;
  int sensor_max;
  const unsigned char output_max;
  volatile unsigned char value;
};

struct analog_out
{
  const unsigned char pin;
  unsigned char value;
};

// # of mux outputs is hardcoded to 2. need to find a way to make this flexible
// without taking up unnecessary sdram
struct digital_mux
{
  unsigned char num_channels; 
  unsigned char num_outs;
  unsigned char select_pin[3]; 
  unsigned char mcu_input_pin[2]; 
  unsigned char value[2];
};

// same issue as above for value[] and num_encoders
struct encoder_set
{
  struct digital_mux *mux;
  unsigned char prev_word;
  unsigned char value[8];
  unsigned char mcu_input_pin_index;
  unsigned char num_encoders;
};

// function prototypes
void init_pins();
void calibrate_analog_ins();
void scan_mux(digital_mux *mux);
void read_analog_in(int);
void process_encoder_data(encoder_set *twddle_enc);

#endif /* MCUIO_H */
