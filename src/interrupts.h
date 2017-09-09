void isr_0();
void isr_1();
void isr_2();

#ifdef ARDUINO_AVR_UNO
void init_interrupts_uno();

void init_interrupts_uno()
{
  // initialize Timer1
  cli();          // disable global interrupts

  //set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  //OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)

  // 1Khz 
  //OCR0A = 255;// = (16*10^6) / (1000*64) - 1 (must be <256)

  // 4Khz
  OCR0A = 63;// = (16*10^6) / (4000*64) - 1 (must be <256)

  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  //OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  OCR1A = 2000;// = (16*10^6) / (1*1024) - 1 (must be <65536)

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  // Set up PWM to 31.25kHz, phase accurate

  // TCCR2*:  timer/counter control registers

  // _BV(): pass it a bit location and it gives you a byte value ( 1 <<(bit) )

  // The Compare Match Output A Mode bits (COM2A1/0) are both at zero which disables this output
   
  // COM2B1 and COM2B0 control the PWM of the digital pin 3
  // in this case COM2B1 is high and COM2B0 is low (0b10), which equates to:
  // 'Clear OC2B on Compare Match when up-counting.' (OC2B = Arduino PIN 3)
  // 'Set OC2B on Compare Match when down-counting.'

  // WGM20/21/22: these 3 bits set the PWM mode - in this case mode 001: PWM phase correct
  // (bits 0 and 1 of TCCR2A, and bit 3 of TCCR2B
  // In Phase Correct PWM mode, the timer counts from 0 to 255 and then back to 0.
  // The PWM output turns high when the counter is in a specific range.

  TCCR2A = _BV(COM2B1) | _BV(WGM20);

  // CS20, CS21 and CS22 (bits 0, 1, and 2 of TCCR2B) select the clock / division ratio
  // in this case 0b001 = 'Division Ratio 1' (i.e no prescaler)

  TCCR2B = _BV(CS20);

  // TIMSK2 (Timer/Counter 2 Interrupt Mask Register) 
  // TOIE2 is an overflow interrupt enable flag
  TIMSK2 = _BV(TOIE2);

  /*
   * OC2B is the output connected to pin 5 of the microprocessor, which correspondsto Arduino’s digital pin 3.
   * OCR2B (Output Compare Register), called PWM_VALUE in Auduino, takes the value of the synthesizer’s output.
   * What it basically does is counting at 16MHz increment a counter from 0 to 255,
   * and when it reaches 255 count down again back to 0. When OCR2B is higher than the counter, OC2B is high and viceversa.
   *
   */
  
  sei();          // enable global interrupts
}

ISR(TIMER0_COMPA_vect)
{
  isr_0();
}

ISR(TIMER1_COMPA_vect)
{
  isr_1();
}

ISR(TIMER2_OVF_vect)
{
  isr_2();
}
#endif



#ifdef ENERGIA
void init_interrupts_energia();

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
