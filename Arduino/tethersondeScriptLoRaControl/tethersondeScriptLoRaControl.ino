// Tethersonde control script (For operation with battery)
// This script first completes a setup for each sensor, calibrates specific sensors, wints for user inputs
// Then begins recodording data to a local SD card once the user inputs have been parsed adn the RTC has been synced

// Libraries
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <SD.h>  // For the SD card data logger

// IMU / BNO085x library
#include <Adafruit_BNO08x.h>
#include <Arduino.h>
// for I2C or UART

// Filter library for AHRS
#include <Adafruit_AHRS_Madgwick.h>

// Barometer Libraries
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

// ThermometerHygormeter Library SHT45
#include "Adafruit_SHT4x.h"

// Thermocouple Library
#include <Adafruit_MAX31856.h>

// Time library
#include "RTClib.h"
RTC_DS3231 rtc;

// Identity and Freq for each sonde
// Uncomment the lines needed for each sonde when uploading
// These frequencies should matach the base station arduino frequencies.

// int identity = 1;                  // Identity input 1
// const long frequency = 903000000;  // Channel 1
int identity = 2;                  // Identity input 2
const long frequency = 906000000;  // Channel 2
// int identity = 3;                  // Identity input 3
// const long frequency = 909000000;  // Channel 3
// int identity = 4;                  // Identity input 4
// const long frequency = 912000000;  // Channel 4
// int identity = 5;                  // Identity input 5
// const long frequency = 915000000;  // Channel 5
// int identity = 6;                  // Identity input 6
// const long frequency = 918000000;  // Channel 6


// IMU varialbes
#define BNO08X_RESET -1

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

// IMU functions and structures ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct euler_t {
  float yaw, pitch, roll;
} ypr;

// Converts the quaternion output to euler varible, yaw, pitch and role
void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false) {

  float sqr = sq(qr);
  float sqi = sq(qi);
  float sqj = sq(qj);
  float sqk = sq(qk);

  ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
  ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
  ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

  //if (degrees) {
  ypr->yaw *= RAD_TO_DEG;
  ypr->pitch *= RAD_TO_DEG;
  ypr->roll *= RAD_TO_DEG;
  // }
}

float yaw, pitch, roll, accX, accY, accZ, gyroX, gyroY, gyroZ, magX, magY, magZ, X, Y, Z;
// Geo magnetic rotation vector varibles these are buoltin quaternion outputs from the IMU
float qr, qi, qj, qk;

// Outputs of the IMU + declination
float azimuth;
float declination;  // declination pulled from GPS coordinates of UofU from the NOAA magnetic calculator system
// Declination is a user input

// SHT45 thermometer variables/ handles
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
float tempSHT, humiditySHT;

// Thermocouple AMP varibles / handles
// use hardware SPI, just pass in the CS pin 6
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(6);
float tempThermocouple;

// SD card varibles 
File dataFile;
const int chipSelect = 7;  // This is the CS pinb selected for the SPI communication

// calibration varibles -- these include the timer and varibles for offset and scaling //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int calibrationTime = 10000;  // 10 seconds

// Barometer global varibles and structres
Adafruit_BMP3XX bmp;
float pressureAtBase;  // make to be a user input
float barometricPressure, baroTemp, altitude, basePressure;

// BLink function variables 
int ledState;

// Communication variables 
int packetSize;
int controlVar = 1;   // initializied at 1 for the switch case that controls the Loop
int g = 103;          //  g for go / literally g is 103 on the ASCII table
int s = 115;          // s for stop This is the value for a new line feed
char dataPacket[20];  // this is for the incoming user input package, itis larger than necessary
int answered1 = 0;    // User input  varible
char* token;          // for parsing the incoming message
int j;                // for indexing the char array dataPacket

///////////////// tethersonde data variables ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// For thermal Anemometer
int sensorOutput;
float windSpeed, windTemp, windADunits, adjustedVoltageOutput, zeroWindV, zero;
int zeroCount = 0;       // For taking the average zero value
const int windPin = A1;  // wind sensor analog pin  hooked up to Wind P sensor "OUT" pin
const int TempPin = A2;  // temp sesnsor analog pin hooked up to Wind P sensor "TMP" pin
// Calibration values
float a = -0.4693;
float b = 0.0147;
float c = 0.5874;
float d = 0.2344;

///////////// Time stamp variables///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long t, t_old, t_old1;
int samplingRate = 10;             // Hz This is as fast as the LORA can handle
int period = 1000 / samplingRate;  // 1 sample per 0.1 s.
long timeStamp;                    // This is the time stamp for syncing the RTC clock
int transmissionRate = 200;        // 5 Hz

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Light varibles
int pin1 = 3;  // red
int pin2 = 4;  // green
int pin3 = 5;  // blue

// Battery varibles
// Battery Voltage Variables
#define BATTERY_PIN A0           // Analog pin used to read the battery voltage
#define VOLTAGE_DIVIDER_RATIO 2  // The voltage divider cuts the voltage in half
#define ADC_RESOLUTION 4096      // 12-bit ADC resolution
#define REFERENCE_VOLTAGE 3300   // Reference voltage in millivolts (3300mV for 3.3V)
int batteryVoltage, batteryPercentage, adcValue, milliVolts;
const int batteryMilliVoltMin = 3400;  //Global variable to hold battery volt minimum from discharge profile. Thershold value
const int batteryMilliVoltMax = 4162;  //Global variable to hold battery volt max from discharge profile. Change to constants later?


// Call for the setup
void setup() {
  // For the setup with the battery just comment out each while (!serial)
  // The battery setup really doesn't require Serial commands so they are not useful except for trouble shooting
  // Blink functions have been utilizied to flag errors for the user, these can , most of the time, be solved from a simple reset

  // Initialize digital pins and for the RGB to be off
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin1, HIGH);  // RGB is pulled HIGH to ground, off
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);

  // 12 bit resolution
  analogReadResolution(12);

  // Blink green upon start setup
  LedBlink(pin2, pin2, 10);

  Serial.begin(115200);

  // Lora setup change the frequency depending on the sonde
  if (!LoRa.begin(frequency)) {
    // Serial.println("Starting LoRa failed!");
    LedBlink(pin1, pin1, 4);  // blink red 2 times
    while (1)
      ;
  }

  // begin RTC
  if (!rtc.begin()) {
    LedBlink(pin1, pin1, 10);  // blink red 5 times
    Serial.flush();
    while (1) delay(10);
  }

  delay(200);

  // SHT setup
  if (!sht4.begin()) {
    LedBlink(pin1, pin1, 10);  // blink red 10 times
    while (1) delay(1);
  }
  delay(200);
  // You can have 3 different precisions, higher precision takes longer
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);  // No heater has been slected

  // BMP setup
  delay(200);
  // hardware I2C mode, can pass in address & alt Wire
  if (!bmp.begin_I2C()) {
    LedBlink(pin1, pin1, 15);  // blink red 15 times
    while (1)
      ;
  }
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  // SD card setup
  delay(200);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    LedBlink(pin1, pin1, 20);  // blink red, 20 times
    while (1)
      ;
  }
  delay(200);

  // This is here to remove the previous save on the SD card
  // This is the file name that will exist on every SD card in the tethersonde fleet
  if (SD.exists("datalog.txt")) {
    SD.remove("datalog.txt");
    delay(200);
  }

  // IMU setup
  delay(200);

  // Try to initialize
  // The IMU often fails upon startup, more than anyother sensor.
  // Resolved by a simple reset until it "Warms up". Reseting is the only way around this.
  if (!bno08x.begin_I2C()) {
    // if (!bno08x.begin_UART(&Serial1)) {  // Requires a device with > 300 byte
    // UART buffer! if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
    LedBlink(pin1, pin1, 40);  // blink red, 40 times
    LedBlink(pin2, pin3, 10);  // Blink cyan to flag for IMU error
    while (1) {
      delay(10);
    }
  }
  delay(200);
  setReports();

  // Thermocouple setup
  if (!maxthermo.begin()) {
    LedBlink(pin1, pin1, 50);  // blink red, 50 times
    delay(200);
    while (1) delay(10);
  }
  maxthermo.setThermocoupleType(MAX31856_TCTYPE_E);
  maxthermo.setConversionMode(MAX31856_CONTINUOUS);

  LedBlink(pin2, pin2, 10);  // Blink green to close out the setup
  // A dealy is included here for time to place the sonde out of the way for its self calibaration
  delay(1000);
}


// Call for the continious loop
void loop() {
  packetSize = LoRa.parsePacket();
  t = millis();
  // Declare beggining of clock
  DateTime now = rtc.now();

  // Function callouts for data collection
  // Thermocouple amp output
  tempThermocouple = maxthermo.readThermocoupleTemperature();
  // IMU output
  IMUControl();
  // Thermal nemometer (windspeed m/s)
  HotWire();
  // Barometric pressure output
  // This includes an altitude measurement aswell sampled at 50Hz automatically
  BaroPressure();
  // SHT45 thermometer and hygrometer output
  TempHumidity();
  // Battery perentage
  BatteryPercent();


  switch (controlVar) {

    case 1:  // Calibration case
      // Calibrate the thermal anemometer by taking a large average of its zero speed value
      // Same for BMP
      // This section loops through and adds the samples together tocreate an average.
      zero = windADunits + zero;
      basePressure = barometricPressure + basePressure;
      // this finds the amount datums produced in this timer
      zeroCount = zeroCount + 1;
      // Break into the next step after the timer has been spent 40s
      if (t - t_old >= calibrationTime) {
        // reset the timer
        t_old = t;
        // Average all of the wind voltage measurements and pressure measurments
        zeroWindV = zero / zeroCount;
        pressureAtBase = 10 * basePressure / zeroCount;  // multiply by 10 to convert to the expected HPa
        // break into the next part of the switch case
        controlVar = 2;
        // Blink blue for calibration end
        LedBlink(pin3, pin3, 30);
      }

      break;

    case 2:  // User input case

      //Question 1, take in and parse the package
      if (answered1 == 0) {
        if (packetSize) {  // This detects if there is a packet

          // read packet
          while (LoRa.available()) {
            dataPacket[j] = LoRa.read();  // Read char array
            j = j + 1;
            answered1 = 1;
            controlVar = 3;  // break into the sending case
          }

          // parse the package by creating tokens
          token = strtok(dataPacket, ",");
          declination = atof(token); // read as a float
          // read the next index in the array
          token = strtok(NULL, ",");
          timeStamp = atol(token); // read as a long
          // sync clock
          rtc.adjust(timeStamp);
          LedBlink(pin1, pin3, 20);  // blink purple upon completion
        }
      }

      break;


    case 3:  //Send and save data here

      // Saving data at 10 Hz
      if (t - t_old >= period) {
        // reset timer
        t_old = t;

        // Transmitting at 5 Hz
        if (t - t_old1 > transmissionRate) {
          // reset timer
          t_old1 = t;
          LoRa.beginPacket();
          LoRa.print(identity);
          LoRa.print(',');
          LoRa.print(azimuth);
          LoRa.print(',');
          LoRa.print(adjustedVoltageOutput);
          LoRa.print(',');
          LoRa.print(tempSHT);
          LoRa.print(',');
          LoRa.print(humiditySHT);
          LoRa.print(',');
          LoRa.print(altitude);
          LoRa.print(',');
          LoRa.print(barometricPressure);
          LoRa.print(',');
          LoRa.print(tempThermocouple);
          LoRa.print(',');
          LoRa.print(batteryPercentage);
          LoRa.print(',');
          LoRa.println(now.unixtime());
          LoRa.endPacket();
        }

        // if the file is available, write to it:
        // You can rename the file if you want but each sonde will contain its identity value with the saved data.
        dataFile = SD.open("datalog.txt", FILE_WRITE);

        // SD card saving code
        if (dataFile) {
          dataFile.print(identity);
          dataFile.print(',');
          dataFile.print(azimuth);
          dataFile.print(',');
          dataFile.print(adjustedVoltageOutput);
          dataFile.print(',');
          dataFile.print(tempSHT);
          dataFile.print(',');
          dataFile.print(humiditySHT);
          dataFile.print(',');
          dataFile.print(altitude);
          dataFile.print(',');
          dataFile.print(barometricPressure);
          dataFile.print(',');
          dataFile.print(tempThermocouple);
          dataFile.print(',');
          dataFile.print(batteryPercentage);
          dataFile.print(',');
          dataFile.println(now.unixtime());
          dataFile.close();
        }
        // if the file isn't open, pop up an error:
        else {
          LedBlink(pin1, pin1, 25);  // blink red, 50 times
        }
      }

      break;

    default:

      // Do nothing

      break;  // This is the end of the major switch case
  }
}
