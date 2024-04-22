void TempHumidity(void) {
  // This code came from the adafruit SHT libray example
  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);  // populate temp and humidity objects with fresh data
  tempSHT = temp.temperature; // C
  humiditySHT = humidity.relative_humidity; // %RH
}