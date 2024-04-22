void IMUControl(void) {
  // outputs the azimuth

// Most of this code came from the bno08x library examples, specficially the More reports example
  if (bno08x.wasReset()) {
    //Serial.print("IMU was reset ");
    setReports();
  }

  if (!bno08x.getSensorEvent(&sensorValue)) {
    return;
  }

  // Switch case is absolutely necissary.
  switch (sensorValue.sensorId) {

    case SH2_GEOMAGNETIC_ROTATION_VECTOR:

      qr = sensorValue.un.geoMagRotationVector.real;
      qi = sensorValue.un.geoMagRotationVector.i;
      qj = sensorValue.un.geoMagRotationVector.j;
      qk = sensorValue.un.geoMagRotationVector.k;

      break;
  }

  quaternionToEuler(qr, qi, qj, qk, &ypr);

  // Collect the yaw output
  yaw = ypr.yaw;

  // Add error offset after post processing the deployment aginast the sonic.
  // convert to a 360 degree circilev with 0 as north
  if (yaw >= 0) {
    yaw = 270 - yaw;
  }
  if (yaw < 0) {
    yaw = 270 - yaw;
  }

  // Add declination
  azimuth = yaw + declination;

  if (azimuth > 360) {
    azimuth -= 360;
  }
  // Azimuth value at this point is complete
}