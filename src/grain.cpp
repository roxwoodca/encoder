#include "Arduino.h"
#include "grain.h"

void grain_signal(grain_synth *synth)
{
  uint8_t  vco1_out;
  uint8_t  vco2_out;
  uint8_t  output;
  //synth->vco1_inc = 16; 

  synth->vco1_acc += synth->vco1_inc;
  synth->vco2_acc += synth->vco2_inc;

  //vco1_out = synth->vco1_acc;
  vco1_out = sin256[synth->vco1_acc];
 
  // if phase is negative then invert value (saw to triangle)
  if (synth->vco1_acc & 0x80)
  {
    //vco1_out = ~vco1_out;
    #ifdef DEBUG_MODE 
    log_debug("inv!",vco1_out,DEC);
    #endif
  }
  else 
  {
    #ifdef DEBUG_MODE 
    log_debug("out",vco1_out,DEC);
    #endif
  }

  output = vco1_out;


  PWM_VALUE = output;
  #ifdef DEBUG_MODE 
  //log_debug("out",output,DEC);
  #endif
}
