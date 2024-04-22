void BatteryPercent(void) {
  // This function calcualtes the battery percentage within the usabel threshold
  adcValue = analogRead(BATTERY_PIN);
  milliVolts = adcValue * REFERENCE_VOLTAGE / ADC_RESOLUTION;
  batteryVoltage = milliVolts * VOLTAGE_DIVIDER_RATIO;  // Directly modify the global variable

  if (batteryVoltage > batteryMilliVoltMax) {
    batteryVoltage = batteryMilliVoltMax;
  }

  // Battery percentage calcualtion
  batteryPercentage = 100 * (batteryVoltage - batteryMilliVoltMin) / (batteryMilliVoltMax - batteryMilliVoltMin);
}