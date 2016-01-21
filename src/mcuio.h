#ifndef MCUIO_H
#ifndef MCUIO_H
// preprocessor stuff
#define NUM_DIGITAL_INS (sizeof digital_ins / sizeof digital_in)
#define NUM_DIGITAL_OUTS (sizeof digital_outs / sizeof digital_out)
#define NUM_ANALOG_INS (sizeof analog_ins / sizeof analog_in)
#define NUM_ANALOG_OUTS (sizeof analog_outs / sizeof analog_out)
#define NUM_DIGITAL_MUX_OUTS 2

// structs
struct digital_in
{
  int pin;
  int value;
} digital_ins[NUM_DIGITAL_INS];

struct digital_out
{
  int pin;
  int value;
} digital_outs[NUM_DIGITAL_OUTS];

struct analog_in
{
  int pin;
  int resolution;
  int sensor_min;
  int sensor_max;
  int output_min;
  int output_max;
  int value;
} analog_ins[NUM_ANALOG_INS];

struct analog_out
{
  int pin;
  int_value;
} analog_outs[NUM_ANALOG_OUTS]

struct digital_mux
{
  int num_channels; 
  digital_out channel_selector[3]; 
  digital_in mux_outs[NUM_DIGITAL_MUX_OUTS]; 
  int values[NUM_DIGITAL_MUX_OUTS];
} digital_mux_1

// function prototypes
void init_pins();
void calibrate_analog_ins();
#endif /* MCUIO_H */
