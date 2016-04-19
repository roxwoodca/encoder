#include "Arduino.h"
#include "grain.h"

void grain_signal(grain_synth *synth)
{
  uint8_t   value;
  uint16_t  output;

  // the sync phase accumulator keeps track of 
  // the start and end of each pair of grains
  // resetting the grain parameters each time it overflows

  // example sync_phase_inc val: 8195 
  // example sync_phase_acc val: 8195,16390,24585,-32756,-24561,-16366,-8171,24...
  synth->sync_phase_acc += synth->sync_phase_inc;

  // if the accumulator overflows
  if (synth->sync_phase_acc < synth->sync_phase_inc) {
    // then it is time to start the next grain
    synth->grain_phase_acc = 0;
    synth->grain_amp = 0x7fff;
    synth->grain2_phase_acc = 0;
    synth->grain2_amp = 0x7fff;
    //LED_PORT ^= 1 << LED_BIT; // Faster than using digitalWrite
  }
  
  // upon each cycle the grain phase accumulator is incremented by the 
  // grain phase increment value (user-configured) 
  // controlling the phase of the grain oscillators
  
  // example grain_phase_inc val: 8195 
  // example grain_phase_acc val: 8195,16390,24585,-32756,-24561,-16366,-8171,24...
  
  synth->grain_phase_acc += synth->grain_phase_inc;
  synth->grain2_phase_acc += synth->grain2_phase_inc;

  // Convert phase into a triangle wave
  // value is just an 8-bit char, so we have to scale down the current grain_phass_acc to set it 
  // example value: 8195 >> 7 = 64
  // 16390 >> 7 = -128
  // 24585 >> 7 = -64
  // -32756 >> 7 = 0 (-256) 
  // -24561 >> 7 = 64 (-192) ~64 = -65
  // -16366 >> 7 = -128 (-128) ~-128 = 127
  // -8171 >> 7 = -64 (-64) ~-64 = 63
  // 24 >> 7 = 0 (0)
  // 8219 >> 7 = 64 (64)
  value = (synth->grain_phase_acc >> 7) & 0xff;

  // if grain_phase is negative then invert value
  if (synth->grain_phase_acc & 0x8000) value = ~value;

  // Multiply by current grain amplitude to get sample
  output = value * (synth->grain_amp >> 8);

  // Repeat for second grain
  value = (synth->grain2_phase_acc >> 7) & 0xff;
  if (synth->grain2_phase_acc & 0x8000) value = ~value;
  output += value * (synth->grain2_amp >> 8);

  // Make the grain amplitudes decay by a factor every sample (exponential decay)
  synth->grain_amp -= (synth->grain_amp >> 8) * synth->grain_decay;
  synth->grain2_amp -= (synth->grain2_amp >> 8) * synth->grain2_decay;

  // Scale output to the available range, clipping if necessary
  output >>= 9;
  if (output > 255) output = 255;

  // Output to PWM (this is faster than using analogWrite)  
  PWM_VALUE = output;
  
  #ifdef DEBUG_MODE
  log_debug("PWM_VAL",output,DEC);
  #endif
}
