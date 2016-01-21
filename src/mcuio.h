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
  int pin;
  int value;
};

struct digital_out
{
  int pin;
  int value;
};

struct analog_in
{
  int pin;
  int resolution;
  int sensor_min;
  int sensor_max;
  int output_min;
  int output_max;
  int value;
};

struct analog_out
{
  int pin;
  int value;
};

struct digital_mux
{
  int num_channels; 
  digital_out channel_selector[3]; 
  digital_in mux_outs[NUM_DIGITAL_MUX_OUTS]; 
  int values[NUM_DIGITAL_MUX_OUTS];
};


// function prototypes
void init_pins();
void calibrate_analog_ins();
void scan_mux();
void read_analog_in(int);
#endif /* MCUIO_H */
