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
  uint16_t  sync_phase_acc;
  uint16_t  sync_phase_inc;
  uint16_t  grain_phase_acc;
  uint16_t  grain2_phase_acc;
  uint16_t  grain_phase_inc;
  uint16_t  grain2_phase_inc;
  uint16_t  grain_amp;
  uint16_t  grain2_amp;
  uint8_t grain_decay;
  uint8_t grain2_decay;
};

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
