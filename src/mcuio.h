// preprocessor stuff
#define NUM_DIGITAL_INS (sizeof digital_ins / sizeof digital_in)
#define NUM_DIGITAL_OUTS (sizeof digital_outs / sizeof digital_out)
#define NUM_ANALOG_INS (sizeof analog_ins / sizeof analog_in)
#define NUM_ANALOG_OUTS (sizeof analog_outs / sizeof analog_out)

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

struct mux
{
  int select_pins[3]; 
  int input_pin; 
}


// functions
void init_pins();
void calibrate_analog_ins();
