#include "Arduino.h"
#include "interrupts.h"
#include "encoder.h"


#ifdef DEBUG_MODE
#include "debug.h"
#endif

digital_mux twddle_mux = { 8, &PORTB, 1, 3, &PINB, 5, 2, { 0, 0 } };
encoder_set twddle_enc = { &twddle_mux, 0, { 0 }, 0, 4, 0, 127 };

void do_midi_thing(int value);

// the setup routine runs once when you press reset:
void setup() 
{
  #ifdef DEBUG_MODE
  Serial.begin(9600);
  #endif

  // set I/O direction register
  DDRB =0x0E;

  init_interrupts_uno();
}

void loop() { }

/* 4KHz */
void isr_0()
{
  scan_mux(&twddle_mux);
  read_encoders(&twddle_enc,do_midi_thing);
}

void do_midi_thing(int value)
{

}

/* 1Hz */
void isr_1()
{
  #ifdef DEBUG_MODE
  log_debug("enc1",twddle_enc.value[0]);
  log_debug("enc2",twddle_enc.value[1]);
  log_debug("enc3",twddle_enc.value[2]);
  log_debug("enc4",twddle_enc.value[3]);
  dump_debugs();
  #endif
}
