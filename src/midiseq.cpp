
#include "Arduino.h"

//constants
const int sensor_pin = A3;
const int digital_ins[] = { 12,13 };
const int num_digital_ins = 2;
const int mux_select_pins[] = { 9,10,11 }; // routed to 11,10,9 (A,B,C) binary control inputs on 4051 where A is the LSB

// variables:
int sensor_value = 0;   // the sensor value
int sensor_min = 1000;  // minimum sensor value
int sensor_max = 100;   // maximum sensor value
int output_min = 0;
int output_max = 127;
unsigned int mux1_state = 0;
unsigned int mux2_state = 0;

// functions
void init_pins();
void print_in_pin_vals(); 
void calibrate();
void scan_mux();

// the setup routine runs once when you press reset:
void setup() 
{
  init_pins();
  
  Serial.begin(9600);

  //calibrate();

  //print_in_pin_vals();
}

void calibrate()
{
  // turn on LED to signal the start of the calibration period:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Please place finger on LDR briefly to calibrate...");

  // calibrate during the first five seconds
   
  while (millis() < 5000) 
  {
    sensor_value = analogRead(sensor_pin);

    // record the maximum sensor value
    if (sensor_value > sensor_max)
    {
      sensor_max = sensor_value;
    }

    // record the minimum sensor value
    if (sensor_value < sensor_min)
    {
      sensor_min = sensor_value;
    }
  }

  // signal the end of the calibration period
  digitalWrite(LED_BUILTIN, LOW);

  // initialize serial communication at 9600 bits per second:
  Serial.println("end of calibration...");
}

void init_pins()
{
  for (int this_pin = 0; this_pin < num_digital_ins; this_pin++) 
  {
    pinMode(digital_ins[this_pin], INPUT);
  } 
  
  for (int this_pin = 0; this_pin < 3 ; this_pin++) 
  {
    pinMode(mux_select_pins[this_pin], OUTPUT);
  } 
    
}

void print_in_pin_vals()
{
  for (int this_pin = 0; this_pin < num_digital_ins; this_pin++)
  {
    Serial.println(digitalRead(digital_ins[this_pin]));
  } 
 
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
  
  Serial.print(mux1_state,BIN);

  Serial.print(" / ");
  Serial.print(mux2_state,BIN);

  Serial.print("\n");

}


void loop()
{
  scan_mux();

/*
  // read the input on analog pin Ax
  int sensor_value = analogRead(sensor_pin);

  int cali_value = map(sensor_value, sensor_min, sensor_max, output_min, output_max);

  // in case the sensor value is outside the range seen during calibration
  cali_value = constrain(cali_value, output_min, output_max);

  // print out the value you read:
  Serial.print(sensor_value);
  Serial.print(" : ");
  Serial.print(cali_value);
  Serial.print("\n"); 

  delay(500); // delay in between reads for stability
*/
}
