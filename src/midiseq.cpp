#include "Arduino.h"
#include <stdio.h>
#include "mcuio.h"

// variables:
unsigned int mux1_state = 0;
unsigned int mux2_state = 0;

// functions
void scan_mux();
void init_interrupts_uno();
void init_interrupts_energia();

// the setup routine runs once when you press reset:
void setup() 
{
  digital_ins = { { 12, 0 }, { 13, 0 } };
  digital_outs = { { 9, 0 }, { 10, 0 }, { 11, 0} };
  analog_ins = { { A3, 127, 1000, 100, 0, 127, 0 } };
  
  setup_pins()

  #ifdef DEBUG_MODE 
  Serial.begin(9600);
  #endif

  //calibrate();

  //print_in_pin_vals();
  
  #ifdef ARDUINO_AVR_UNO 
    init_interrupts_uno();
  #elif ENERGIA 
    init_interrupts_energia();
  #endif

}

/********************ARDUINO-SPECIFIC*****************/
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
  scan_mux();
}
#endif
/*******************END OF ARDUINO-SPECIFIC***********/


/*******************MSP430-SPECIFIC*******************/
#ifdef ENERGIA
void init_interrupts_energia()
{
  TACCTL0 |= CCIE;	//Enable Interrupts on Timer
  TACCR0 = 500;	//Number of cycles in the timer
  TACTL |= TASSEL_1;	//Use ACLK as source for timer
  TACTL |= MC_1;	//Use UP mode timer
  __enable_interrupt(); //Set GIE in SR
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void myTimerISR(void)
{
  scan_mux();
}
#endif
/******************END OF MSP430-SPECIFIC*************/

void calibrate_analog_ins()
{
  
  // turn on LED to signal the start of the calibration period:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  #ifdef DEBUG_MODE
  Serial.println("Calibrating. Please sweep all analog controls...");
  #endif

  int i = 0;
  int sensor_value;

  // calibrate during the first five seconds
  while (millis() < 5000) 
  {
    for (i = 0; i < NUM_ANALOG_INS; i++)
    {
      sensor_value = analogRead(analog_ins[i].pin);

      // record the maximum sensor value
      if (sensor_value > analog_ins[i].sensor_max)
      {
        analog_ins[i].sensor_max = sensor_value;
      }

      // record the minimum sensor value
      if (sensor_value < analog_ins[i].sensor_min)
      {
        analog_ins[i].sensor_min = sensor_value;
      }
    }
  }

  // signal the end of the calibration period
  digitalWrite(LED_BUILTIN, LOW);

  #ifdef DEBUG_MODE
  Serial.println("end of calibration...");
  #endif
}

void scan_mux()
{
  int mux1_cur_bit,mux2_cur_bit;
  mux1_state = 0;
  mux2_state = 0;
  for(int i=0; i<8; i++)
  {
    // shift required bit i places right to LSB position and use bitwise and to identify it's value
    digitalWrite(mux_select_pins[0], i & 0x1);
    digitalWrite(mux_select_pins[1], (i>>1) & 0x1);
    digitalWrite(mux_select_pins[2], (i>>2) & 0x1);
    mux1_cur_bit = digitalRead(digital_ins[0]);
    mux2_cur_bit = digitalRead(digital_ins[1]);
    mux1_state = (mux1_state<<1) | mux1_cur_bit;
    mux2_state = (mux2_state<<1) | mux2_cur_bit;
  }

#ifdef DEBUG_MODE  
  Serial.print(mux1_state,BIN);

  Serial.print(" / ");
  Serial.print(mux2_state,BIN);

  Serial.print("\n");
#endif
}

void analogue_pin_read(sensor_pin,sensor_min,sensor_max)
{

  // read the input on analog pin Ax
  int sensor_value = analogRead(sensor_pin);

  int cali_value = map(sensor_value, sensor_min, sensor_max, output_min, output_max);

  // in case the sensor value is outside the range seen during calibration
  cali_value = constrain(cali_value, output_min, output_max);

#ifdef DEBUG_MODE
  // print out the value you read:
  Serial.print(sensor_value);
  Serial.print(" : ");
  Serial.print(cali_value);
  Serial.print("\n"); 
#endif
}

void loop()
{
  

}
