void init_pins()
{
  int i;

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
    pinMode(analog_ins[i].pin, INPUT;
  }

  for (i = 0; i < NUM_ANALOG_OUT; i++)
  {
    pinMode(analog_outs[i].pin, OUTPUT;
  }
}

void calibrate_analog_ins()
{
  #ifdef LED_BUILTIN
  // turn on LED to signal the start of the calibration period:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  #endif  

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

  #ifdef LED_BUILTIN
  // signal the end of the calibration period
  digitalWrite(LED_BUILTIN, LOW);
  #endif

  #ifdef DEBUG_MODE
  Serial.println("end of calibration...");
  #endif
}
