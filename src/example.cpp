#include "Arduino.h"
#include "interrupts.h"
#include "encoder.h"
#include "num_disp.h"

#ifdef DEBUG_MODE
#include "debug.h"
#endif

digital_mux   twddle_mux   = { 8, &PORTD, 5, 3, &PINB, 4, 2, { 0, 0 } };
encoder_set   twddle_enc   = { &twddle_mux, 0, { 0 }, 1, 4, 0, 127 };
numeric_display twddle_num_disp = { 3, &PORTB, 0, 4, &PORTD, 2, 0, 0, 32, 0 }; 

void do_midi_thing(int value);

// the setup routine runs once when you press reset:
void setup() 
{
  #ifdef DEBUG_MODE
  Serial.begin(9600);
  #endif

  // set I/O direction register
  DDRB =0x0F;
  DDRD =0xFE;
  init_interrupts_uno();
 
}

void loop() { }

/* 4KHz */
void isr_0()
{
  scan_mux(&twddle_mux);
  read_encoders(&twddle_enc,do_midi_thing);
  num_disp_write(twddle_enc.value[0],&twddle_num_disp);
  //num_disp_write(124,&twddle_num_disp);
}

void do_midi_thing(int value)
{

}

/* 1Hz */
void isr_1()
{
  #ifdef DEBUG_MODE
  /*
  log_debug("enc1",twddle_enc.value[0]);
  log_debug("enc2",twddle_enc.value[1]);
  log_debug("enc3",twddle_enc.value[2]);
  log_debug("enc4",twddle_enc.value[3]);
  */
  dump_debugs();
  
  #endif
}
