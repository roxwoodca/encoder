#ifndef GRAIN_H
#define GRAIN_H

#ifdef DEBUG_MODE
#include "Arduino.h"
#include "debug.h"
#endif

#include <stdint.h>

#define PWM_PIN       3 
#define PWM_VALUE     OCR2B
#define PWM_INTERRUPT TIMER2_OVF_vect

struct grain_synth
{
  uint8_t  vco1_inc;
  uint8_t  vco1_acc;
  uint8_t  vco2_inc;
  uint8_t  vco2_acc;
};
PROGMEM const unsigned char sin256[]  = {127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,223,225,227,229,231,233,234,236,238,239,240,242,243,244,245,247,248,249,249,250,251,252,252,253,253,253,254,254,254,254,254,254,254,253,253,253,252,252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,229,227,225,223,221,219,217,215,212,210,208,205,203,200,198,195,192,190,187,184,181,178,176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,76,73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,16,18,20,21,23,25,27,29,31,33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124};

/*
// sync phase accumulator 
//uint16_t  sync_phase_acc;

// sync phase increment (16-bit number set by user) 
// alters frequency
//uint16_t  sync_phase_inc;

// grain phase accumulators
//uint16_t  grain_phase_acc;
//uint16_t  grain2_phase_acc;

// grain phase increments (16-bit numbers set by user)
//uint16_t  grain_phase_inc;
//uint16_t  grain2_phase_inc;

// grain amplitudes
//uint16_t  grain_amp;
//uint16_t  grain2_amp;

// grain decay factors (16-bit numbers set by user)
//uint8_t grain_decay;
//uint8_t grain2_decay;
*/

void grain_signal(grain_synth *synth);

#endif /* GRAIN_H */
