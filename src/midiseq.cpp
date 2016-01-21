#include "Arduino.h"
#include <stdio.h>
#include "debug.h"
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
  digital_ins = { { 12, 0 }, { 13, 0 } };
  digital_outs = { { 9, 0 }, { 10, 0 }, { 11, 0} };
  analog_ins = { { A3, 127, 1000, 100, 0, 127, 0 } };
  digital_mux_1 = { 8, { 9, 10, 11 }, { 12, 13 }, { 0, 0 } };  

  // set pin directions 
  init_pins()

  // user will need to sweep pots etc
  calibrate_analog_ins();

  // platform-specific interrupt init
  #ifdef ARDUINO_AVR_UNO 
    init_interrupts_uno();
  #elif ENERGIA 
    init_interrupts_energia();
  #endif

}

void isr_1()
{
  scan_mux();
  read_analog_in(0);
}

void loop()
{
  

}
