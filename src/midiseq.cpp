#include "Arduino.h"

#ifdef DEBUG_MODE
#include "debug.h"
#endif

#include "mcuio.h"
#include "interrupts.h"

digital_in digital_ins[] = { { 12, 0 }, { 13, 0 } };
digital_out   digital_outs[] = { { 9, 0 }, { 10, 0 }, { 11, 0} };
analog_in analog_ins[] = { { A3, 127, 1000, 100, 127, 0 } };
analog_out analog_outs[] = {};
digital_mux digital_mux_1 = { 8, { 9, 10, 11 }, { 12, 13 }, { 0, 0 } };

// the setup routine runs once when you press reset:
void setup() 
{
  #ifdef DEBUG_MODE
  Serial.begin(9600);
  #endif

  // set pin directions 
  init_pins();
  //pinMode(LED_BUILTIN, OUTPUT);

  // user will need to sweep pots etc
  //calibrate_analog_ins();

  // platform-specific interrupt init
  #ifdef ARDUINO_AVR_UNO 
    init_interrupts_uno();
  #elif ENERGIA 
    init_interrupts_energia();
  #endif
}

/* 2KHz */
void isr_0()
{
  scan_mux();
  //read_analog_in(0);
}

/* 1Hz */
void isr_1()
{
  log_debug("mx1vl1",digital_mux_1.value[0]);
  log_debug("mx1vl2",digital_mux_1.value[1]);
  log_debug("ana0val",analog_ins[0].value);
  
  #ifdef DEBUG_MODE
  dump_debugs();
  #endif
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void loop()
{

}

#include "mcuio.cpp"
