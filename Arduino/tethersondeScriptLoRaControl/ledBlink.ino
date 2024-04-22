int LedBlink(int pin1, int pin2, int count) {
  // loop through 30 blinks
  for (int i = 0; i <= count; i = i) {
  
    // if the LED is off turn it on and vice-versa:
    if (ledState == HIGH) {
      ledState = LOW;
    } else {
      ledState = HIGH;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(pin1, ledState);
    digitalWrite(pin2, ledState);

    delay(100);
    i++;  // step through the for loop
  }
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
}