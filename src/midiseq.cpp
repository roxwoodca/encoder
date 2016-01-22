#include "Arduino.h"
#include "debug.h"
#include "mcuio.h"
#include "interrupts.h"

digital_in digital_ins[] = { { 12, 0 }, { 13, 0 } };
digital_out   digital_outs[] = { { 9, 0 }, { 10, 0 }, { 11, 0} };
analog_in analog_ins[] = { { A3, 127, 1000, 100, 0, 127, 0 } };
analog_out analog_outs[] = {};
digital_mux digital_mux_1 = { 8, { 9, 10, 11 }, { 12, 13 }, { 0, 0 } };
float interrupt_interval = 1000;

// the setup routine runs once when you press reset:
void setup() 
{
  // set pin directions 
  init_pins();
  pinMode(LED_BUILTIN, OUTPUT);

  // user will need to sweep pots etc
  calibrate_analog_ins();

  // platform-specific interrupt init
  #ifdef ARDUINO_AVR_UNO 
    init_interrupts_uno(interrupt_interval);
  #elif ENERGIA 
    init_interrupts_energia(interrupt_interval);
  #endif
}

void isr_1()
{
  scan_mux();
  read_analog_in(0);
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void loop()
{
 ;; 

}

#include "mcuio.cpp"
