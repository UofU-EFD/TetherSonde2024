void HotWire(void) {
  // this function controls the thermal anemometer output and fuses the ambinet temperature to help with the steadiness of the output
  // Output voltage
  // Here we will apply a regression
  // 12 bit resolution 4096
  // 3.3 volt ouput
  // around a baseline of 1.5V at zero air speed
  sensorOutput = analogRead(windPin);
  windADunits = (sensorOutput)*3.3 / 4096;
  windTemp = maxthermo.readThermocoupleTemperature();  // Instantaneous thermocoule temp in celcius
  // Seperated so that we may zero out the voltage output so that we dont have NAN
  // ZeroWind is determined by the inital calibration
  adjustedVoltageOutput = windADunits - zeroWindV;

  // keep the output from returning a non zero output
  if (adjustedVoltageOutput < 0.) {
    adjustedVoltageOutput = 0.;
  }

  // calculate windspeed (m/s)
  windSpeed = pow(((adjustedVoltageOutput - a - b * windTemp) / c), (1 / d));
}
