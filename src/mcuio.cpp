#include "Arduino.h"
#include "interrupts.h"
#include "mcuio.h"
#include "config.h"

#ifdef DEBUG_MODE
#include "debug.h"
#endif


// the setup routine runs once when you press reset:
void setup() 
{
  #ifdef DEBUG_MODE
  Serial.begin(9600);
  #endif

  // set pin directions 
  init_pins();

  // user will need to sweep pots etc
  //calibrate_analog_ins();

  // platform-specific interrupt init
  #ifdef ARDUINO_AVR_UNO 
    init_interrupts_uno();
  #elif ENERGIA 
    init_interrupts_energia();
  #endif
}

/* 2KHz */
void isr_0()
{
  //may as well scan the whole mux once for all controller objects
  scan_mux(&twddle_mux);
  process_encoder_data(&twddle_enc);
  //read_analog_in(0);
}

/* 1Hz */
void isr_1()
{
  #ifdef DEBUG_MODE

  //log_debug("mx1vl1",twddle_mux.value[0],2);
  //log_debug("mx1vl2",twddle_mux.value[1],2);
  //log_debug("ana0val",analog_ins[0].value);

  log_debug("twddleA",twddle_enc.value[0]); 
  log_debug("twddleB",twddle_enc.value[1]); 
  log_debug("twddleC",twddle_enc.value[2]); 
  log_debug("twddleD",twddle_enc.value[3]); 

  dump_debugs();
  #endif
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void loop()
{

}

void init_pins()
{
  unsigned int i;

  for (i = 0; i < NUM_DIGITAL_INS; i++)
  {
    pinMode(digital_ins[i].pin, INPUT);
  }

  for (i = 0; i < NUM_DIGITAL_OUTS; i++)
  {
    pinMode(digital_outs[i].pin, OUTPUT);
  }

  for (i = 0; i < NUM_ANALOG_INS; i++)
  {
    pinMode(analog_ins[i].pin, INPUT);
  }

  for (i = 0; i < NUM_ANALOG_OUTS; i++)
  {
    pinMode(analog_outs[i].pin, OUTPUT);
  }

}

void calibrate_analog_ins()
{
  #ifdef LED_BUILTIN
  // turn on LED to signal the start of the calibration period:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  #endif  

  unsigned int i;
  int sensor_value;

  // calibrate during the first five seconds
  while (millis() < 5000)
  {
    // loop through analog_ins, extending min and max values
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

  #ifdef LED_BUILTIN
  // signal the end of the calibration period
  digitalWrite(LED_BUILTIN, LOW);
  #endif
}

/* We are using a single mux object right now, with a 3-bit channel selector. 
 * That 'mux' may be made up of multiple mux ic's each with own output
 * as long as each uses the same channel selector and has the same number of channels .
 * For now we are hard-coded to 2 x 4051 
 * This function steps through the channels of each mux (using the shared channel selector)
 * shifting each bit to the left to eventually read a full byte 
 */
void scan_mux(digital_mux *mux)
{
  // some local variables to store individual bits
  int cur_bit;

  unsigned int i,j;

  // zero out the mux values or else we get some crazy shit happening
  for(j=0; j < mux->num_outs; j++)
  {
    mux->value[j] = 0;
  }

  // loop through mux out channels in parallel 
  for(i=0; i < mux->num_channels; i++)
  {
    // shift required bit i places right to LSB position and use bitwise and to identify it's value
    digitalWrite(mux->select_pin[0], i & 0x1);
    digitalWrite(mux->select_pin[1], (i>>1) & 0x1);
    digitalWrite(mux->select_pin[2], (i>>2) & 0x1);
   
    // each set of mux bits is stored in a byte
    for(j=0; j < mux->num_outs; j++)
    {
      cur_bit = digitalRead(mux->mcu_input_pin[j]);
      mux->value[j] = (mux->value[j]<<1) | cur_bit;
    }
  }
}

// so this will probably be a lot of bitwise stuff i suppose
// we're going to be comparing encoder values in pairs to their previous value
void process_encoder_data(encoder_set *enc_set)
{
  // get current mux word
  unsigned char cur_word = enc_set->mux->value[enc_set->mcu_input_pin_index]; 

  // unsigned char cur_bit_a, cur_bit_b, prev_bit_a, prev_bit_b;

  unsigned char prev_val, cur_val;

  unsigned int i;

  // log_debug("word",cur_word,BIN);

  for(i=0; i < enc_set->num_encoders*2; i=i+2)
  {
    prev_val = enc_set->prev_word >> i & 0b11;
    cur_val = cur_word >> i & 0b11; 
    
    //log_debug("index",i);
    //log_debug("value",cur_val,BIN); 
   
    if (cur_val == prev_val)
    {
      // do nothing. no change
    } 
    // encoder has been turned clockwise
    else if 
        ((prev_val == 0b00 && cur_val == 0b01) ||
	(prev_val == 0b01 && cur_val == 0b11)  ||
	(prev_val == 0b11 && cur_val == 0b10)  ||
        (prev_val == 0b10 && cur_val == 0b00))
    {
       // increment the interpreted encoder value
       log_debug("cw",i);
       enc_set->value[i/2]++;  
    }
    // encoder has been turned anti-clockwise
    else if 
        ((prev_val == 0b00 && cur_val == 0b10) ||
	(prev_val == 0b10 && cur_val == 0b11)  ||
	(prev_val == 0b11 && cur_val == 0b01)  ||
        (prev_val == 0b01 && cur_val == 0b00))
    {
       // increment the interpreted encoder value
       log_debug("ccw",i);
       enc_set->value[i/2]--;  
    }
  } 

  //record the current word for comparison 
  enc_set->prev_word = cur_word; 
}


//read, recalibrate, and store the current value of an analog input  
void read_analog_in(int index)
{
  // read the input on analog pin Ax
  int sensor_value = analogRead(analog_ins[index].pin);

  // recalibrate value according to desired scale
  int cali_value = map(sensor_value,
                       analog_ins[index].sensor_min,
                       analog_ins[index].sensor_max,
                       0,
                       analog_ins[index].output_max);

  // in case the sensor value is outside the range seen during calibration
  cali_value = constrain(cali_value, 0, analog_ins[index].output_max);

  // store the new value;
  analog_ins[index].value = cali_value;
}
