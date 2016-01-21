#include "Arduino.h"
#include <stdio.h>
#include "debug.h"
#include "mcuio.h"

/********************ARDUINO*INTERRUPTS*****************/
#ifdef ARDUINO_AVR_UNO
void init_interrupts_uno()
{
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B

  // set compare match register to desired timer count (100ms):
  OCR1A = 156.15;
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  sei();          // enable global interrupts
}

ISR(TIMER1_COMPA_vect)
{
  isr_1();
}
#endif
/*******************END OF ARDUINO-SPECIFIC***********/


/*******************MSP430*INTERRUPTS*******************/
#ifdef ENERGIA
void init_interrupts_energia()
{
  TACCTL0 |= CCIE;	//Enable Interrupts on Timer
  TACCR0 = 500; 	//Number of cycles in the timer
  TACTL |= TASSEL_1;	//Use ACLK as source for timer
  TACTL |= MC_1;	//Use UP mode timer
  __enable_interrupt(); //Set GIE in SR
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void myTimerISR(void)
{
  isr_1();
}
#endif
/******************END OF MSP430-SPECIFIC*************/
