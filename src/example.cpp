#include "Arduino.h"
#include "interrupts.h"
#include "encoder.h"
#include "num_disp.h"
#include "grain.h"

#ifdef DEBUG_MODE
#include "debug.h"
#endif

void mom_on_action(char);
void mom_off_action(char switch_index);
void mom_single_click(char switch_index);
void mom_double_click(char switch_index);
void mom_long_closed(char switch_index);

digital_mux   twddle_mux   =      { 8, &PORTD, 5, 3, &PINB, 4, 2, { 0, 0 } };
encoder_set   twddle_enc   =      { &twddle_mux, 0, { { 0 }, { 0 }, { 0 }, { 0 } }, 1, 4, 0, 127, 0, 0 };
momentary_session  mom_session  = { { }, { } }; 
momentary_set twddle_mom   =      { &twddle_mux, 0, 0, 4, mom_on_action, mom_off_action, mom_single_click, mom_double_click, mom_long_closed, 4, 1000, &mom_session }; 
numeric_display twddle_num_disp = { 3, &PORTB, 0, 4, &PORTC, 0, 0, 0, 32, 0, 0 }; 
grain_synth cereal_killer =       { 0, 8195, 0, 0, 8195, 4367, 32766, 16383, 0, 0 };

void do_midi_thing(int value);

void do_midi_thing(int value)
{

}


void mom_on_action(char switch_index)
{
  #ifdef DEBUG_MODE
  log_debug("swclsd",1,DEC);
  #endif

  // set cur encoder
  twddle_enc.cur_encoder = switch_index;

  // Start strobe effect
  twddle_num_disp.is_flashing = 1;
}

void mom_off_action(char switch_index)
{
  #ifdef DEBUG_MODE
  log_debug("swopen",1,DEC);
  #endif

  // Stop strobe effect
  twddle_num_disp.is_flashing = 0;
  twddle_num_disp.disable = 0;
}

void mom_single_click(char switch_index)
{
  #ifdef DEBUG_MODE
  log_debug("sglclk",1,DEC);
  #endif

  // Switch to encoder at switch_index
}

void mom_double_click(char switch_index)
{
  #ifdef DEBUG_MODE
  log_debug("dblclk",1,DEC);
  #endif

  // Switch to parameter bank: switch_index
  set_cur_bank(&twddle_enc, switch_index);
}

void mom_long_closed(char switch_index)
{
  #ifdef DEBUG_MODE
  log_debug("lngclsd",1,DEC);
  #endif

  // Save parameter at switch_index in current parameter bank
}

// the setup routine runs once when you press reset:
void setup() 
{
  #ifdef DEBUG_MODE
  Serial.begin(9600);
  #endif

  // set I/O direction register
  DDRB =0x0F;
  DDRD =0xFE;
  DDRC =0x07;
  init_interrupts_uno();
  pinMode(PWM_PIN,OUTPUT);
 
}

void loop() { }

/* 2KHz */
void isr_0()
{
  scan_mux(&twddle_mux);
  read_encoders(&twddle_enc,do_midi_thing);
  read_momentary_switches(&twddle_mom);
  num_disp_write(twddle_enc.value[twddle_enc.cur_bank][twddle_enc.cur_encoder],&twddle_num_disp);

  cereal_killer.sync_phase_inc=twddle_enc.value[0][0]*100;
  cereal_killer.grain_phase_inc=twddle_enc.value[0][1]*100;
  cereal_killer.grain2_phase_inc=twddle_enc.value[0][2]*100;
  cereal_killer.grain_amp=twddle_enc.value[0][3]*100;
 
  #ifdef DEBUG_MODE 
  /*
  log_debug("val 0",twddle_enc.value[0][0],DEC);
  log_debug("val 1",twddle_enc.value[0][1],DEC);
  log_debug("val 2",twddle_enc.value[0][2],DEC);
  log_debug("val 3",twddle_enc.value[0][3],DEC);
  */
  #endif
}

void isr_2()
{
  grain_signal(&cereal_killer);
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

  if (twddle_num_disp.is_flashing == 1)
  {
    //num_disp_enable_cur_digit(&twddle_num_disp);
    num_disp_flash(&twddle_num_disp);
  }
}
