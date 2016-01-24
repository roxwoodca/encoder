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
  char pin;
  char value;
};

struct digital_out
{
  char pin;
  char value;
};

struct analog_in
{
  unsigned char pin;
  unsigned char resolution;
  int sensor_min;
  int sensor_max;
  unsigned char output_max;
  unsigned char value;
};

struct analog_out
{
  unsigned char pin;
  unsigned char value;
};

struct digital_mux
{
  unsigned char num_channels; 
  unsigned char channel_selector[3]; 
  unsigned char mux_out[NUM_DIGITAL_MUX_OUTS]; 
  unsigned char value[NUM_DIGITAL_MUX_OUTS];
};


// function prototypes
void init_pins();
void calibrate_analog_ins();
void scan_mux();
void read_analog_in(int);

#endif /* MCUIO_H */
