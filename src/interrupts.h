void isr_0();
void isr_1();

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
  OCR0A = 255;// = (16*10^6) / (1000*64) - 1 (must be <256)

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
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

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
