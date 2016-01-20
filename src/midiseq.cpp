#include "Arduino.h"
#include <stdio.h>
#include "mcuio.h"

// variables:
unsigned int mux1_state = 0;
unsigned int mux2_state = 0;

// functions
void scan_mux();
void init_interrupts_uno();
void init_interrupts_energia();

// the setup routine runs once when you press reset:
void setup() 
{
  #ifdef DEBUG_MODE 
  Serial.begin(9600);
  #endif

  digital_ins = { { 12, 0 }, { 13, 0 } };
  digital_outs = { { 9, 0 }, { 10, 0 }, { 11, 0} };
  analog_ins = { { A3, 127, 1000, 100, 0, 127, 0 } };
  
  setup_pins()

  calibrate_analog_ins();

  #ifdef ARDUINO_AVR_UNO 
    init_interrupts_uno();
  #elif ENERGIA 
    init_interrupts_energia();
  #endif

}

void scan_mux()
{
  int mux1_cur_bit,mux2_cur_bit;
  mux1_state = 0;
  mux2_state = 0;
  for(int i=0; i<8; i++)
  {
    // shift required bit i places right to LSB position and use bitwise and to identify it's value
    digitalWrite(mux_select_pins[0], i & 0x1);
    digitalWrite(mux_select_pins[1], (i>>1) & 0x1);
    digitalWrite(mux_select_pins[2], (i>>2) & 0x1);
    mux1_cur_bit = digitalRead(digital_ins[0]);
    mux2_cur_bit = digitalRead(digital_ins[1]);
    mux1_state = (mux1_state<<1) | mux1_cur_bit;
    mux2_state = (mux2_state<<1) | mux2_cur_bit;
  }

#ifdef DEBUG_MODE  
  log_debug("mux1_state",mux1_state);
  log_debug("mux2_state",mux2_state);
#endif
}

void analog_in_value_get(index)
{
  // read the input on analog pin Ax
  int sensor_value = analogRead(analog_ins[index].pin);

  // calibrate value according to desired scale
  int cali_value = map(sensor_value,
                       analog_ins[index].sensor_min,
                       analog_ins[index].sensor_max,
                       analog_ins[index].output_min,
                       analog_ins[index].output_max);

  // in case the sensor value is outside the range seen during calibration
  cali_value = constrain(cali_value, analog_ins[index].output_min, analog_ins[index].output_max);

#ifdef DEBUG_MODE
  log_debug("sensor_value",sensor_value)
  log_debug("cali_value",cali_value")
#endif
}

void isr_1()
{
  scan_mux();
  scan_mux();
}

void loop()
{
  

}
