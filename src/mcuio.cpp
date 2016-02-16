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

  // set I/O direction register
  DDRB =0x30;

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
 * This function steps through the channels of each mux (using the shared channel selector)
 * shifting each bit to the left to eventually read a full byte 
 */
void scan_mux(digital_mux *mux)
{
  unsigned int i, j, cur_bit;

  // loop through mux out channels in parallel 
  for(i=0; i < mux->num_channels; i++)
  {
    // use bitmask to modify selector pin bits
    *mux->selector_port |=(i << mux->selector_pin_offset);
   
    // loop through input ports, shifting to get value of each pin
    //  and shifting them onto a byte for each input 
    for (j=0; j < mux->num_input_pin; j++)
    {
      cur_bit = ((*mux->input_port >> (mux->input_pin_offset+j)) & 0x01);
      mux->value[j] = ((mux->value[j]<<1) | cur_bit);
    }
  }
}

// compare encoder values in pairs to their previous value
void process_encoder_data(encoder_set *enc_set)
{
  // get current mux word
  unsigned char cur_word = enc_set->mux->value[enc_set->mcu_input_pin_index]; 
  unsigned char i, prev_val, cur_val;

  for(i=0; i < enc_set->num_encoders*2; i=i+2)
  {
    prev_val = enc_set->prev_word >> i & 0b11;
    cur_val = cur_word >> i & 0b11; 

    //if no values have changed or the previous value stored has not yet expired
    if (cur_val == prev_val || enc_set->cur_expiry[i/2] < enc_set->expiry_life)
    {
      //decrement the expiry
      if (enc_set->cur_expiry > 0)
      {
        enc_set->cur_expiry[i/2]--;
      }
      //unless it is at 0 already, in which case reset the expiry counter
      else
      { 
        enc_set->cur_expiry[i/2] = enc_set->expiry_life;
      }
    } 
    // encoder has been turned clockwise
    else if 
        ((prev_val == 0b00 && cur_val == 0b01) ||
	(prev_val == 0b01 && cur_val == 0b11)  ||
	(prev_val == 0b11 && cur_val == 0b10)  ||
        (prev_val == 0b10 && cur_val == 0b00))
    {
       // increment the associated value
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
       // decrement the associated value
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
