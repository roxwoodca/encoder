#include "Arduino.h"
#include "debug.h"
#include "mcuio.h"
#include "interrupts.h"

digital_in digital_ins[] = { { 12, 0 }, { 13, 0 } };
digital_out   digital_outs[] = { { 9, 0 }, { 10, 0 }, { 11, 0} };
analog_in analog_ins[] = { { A3, 127, 1000, 100, 0, 127, 0 } };
analog_out analog_outs[] = {};
digital_mux digital_mux_1 = { 8, { 9, 10, 11 }, { 12, 13 }, { 0, 0 } };


// the setup routine runs once when you press reset:
void setup() 
{
  // set pin directions 
  init_pins();

  // user will need to sweep pots etc
  calibrate_analog_ins();

  // platform-specific interrupt init
  #ifdef ARDUINO_AVR_UNO 
    init_interrupts_uno();
  #elif ENERGIA 
    init_interrupts_energia();
  #endif
}

void isr_1()
{
  scan_mux();
  read_analog_in(0);
}

void loop()
{
 ;; 

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
void scan_mux()
{
  // some local variables to store individual bits
  int mux1_cur_bit,mux2_cur_bit;

  // set the mux values to zero or else we get some crazy shit happening
  digital_mux_1.value[0] = 0;
  digital_mux_1.value[1] = 0;  

  unsigned int i;
  // loop through 
  for(i=0; i < digital_mux_1.num_channels; i++)
  {
    // shift required bit i places right to LSB position and use bitwise and to identify it's value
    digitalWrite(digital_mux_1.channel_selector[0], i & 0x1);
    digitalWrite(digital_mux_1.channel_selector[1], (i>>1) & 0x1);
    digitalWrite(digital_mux_1.channel_selector[2], (i>>2) & 0x1);
    mux1_cur_bit = digitalRead(digital_mux_1.mux_out[0]);
    mux2_cur_bit = digitalRead(digital_mux_1.mux_out[1]);
    digital_mux_1.value[0] = (digital_mux_1.value[0]<<1) | mux1_cur_bit;
    digital_mux_1.value[1] = (digital_mux_1.value[1]<<1) | mux2_cur_bit;
  }

#ifdef DEBUG_MODE  
  log_debug("mux_1_value1",digital_mux_1.value[0]);
  log_debug("mux_1_value2",digital_mux_1.value[1]);
#endif
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
                       analog_ins[index].output_min,
                       analog_ins[index].output_max);

  // in case the sensor value is outside the range seen during calibration
  cali_value = constrain(cali_value, analog_ins[index].output_min, analog_ins[index].output_max);

  // store the new value;
  analog_ins[index].value = cali_value;

  #ifdef DEBUG_MODE
  log_debug("sensor_value",sensor_value)
  log_debug("cali_value",cali_value)
  #endif
}
