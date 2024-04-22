// This is the tab that contorls the Barometer that reads our atmospheric pressure
// Calibration note: pressure at base is a calibrated value, ratehre a zeroed value
// for the altutide calculation
// The information for this section came from the adafruit BMP library
void BaroPressure(void) {
  if (!bmp.performReading()) {
    LedBlink(pin1, pin1, 10);  // blink red 10 times
    return;
  }
  barometricPressure = bmp.pressure / 1000.0;   // kPa
  altitude = bmp.readAltitude(pressureAtBase);  // meters
  baroTemp = bmp.temperature;                   // Celcius
}
