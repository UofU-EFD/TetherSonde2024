// Here is where you define the sensor outputs you want to receive from the IMU
// THis function is set to output geomagnetic vector data (COMPASS)
// This came from the bno08x library for declaring which report we want to output from the IMU
// The IMU really only likes to use one of these at a time.
void setReports(void) {
  // SH2_GEOMAGNETIC_ROTATION_VECTOR is the specific callout
  // from the IMU library that selects magnemometer data

  if (!bno08x.enableReport(SH2_GEOMAGNETIC_ROTATION_VECTOR)) {
    Serial.println("Could not enable game vector");
  }

  // if (!bno08x.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED)) {
  //   Serial.println("Could not enable game vector");
  // }

  // if (!bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED)) {
  //   Serial.println("Could not enable game vector");
  // }

  // if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
  //   Serial.println("Could not enable game vector");
  // }

  // if (!bno08x.enableReport(SH2_RAW_MAGNETOMETER)) {
  //   Serial.println("Could not enable raw magnetometer");
  // }
}