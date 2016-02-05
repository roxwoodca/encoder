#ifndef MCUIO_H
#define MCUIO_H

#define NUM_DIGITAL_INS (sizeof digital_ins / sizeof digital_ins[0])
#define NUM_DIGITAL_OUTS (sizeof digital_outs / sizeof digital_outs[0])
#define NUM_ANALOG_INS (sizeof analog_ins / sizeof analog_ins[0])
#define NUM_ANALOG_OUTS (sizeof analog_outs / sizeof analog_outs[0])
#define NUM_DIGITAL_MUX_OUTS 2

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

struct digital_mux
{
  const unsigned char num_channels; 
  const unsigned char channel_selector[3]; 
  const unsigned char mux_out[NUM_DIGITAL_MUX_OUTS]; 
  volatile unsigned char value[NUM_DIGITAL_MUX_OUTS];
};

struct rotary_encoder
{
  char prev_val;
  char cur_val;
  char direction;
};

// function prototypes
void init_pins();
void calibrate_analog_ins();
void scan_mux(digital_mux *mux);
void read_analog_in(int);
void process_encoder_data(struct digital_mux mux, struct rotary_encoder *encoder, char num_pairs, char set_index);

#endif /* MCUIO_H */
