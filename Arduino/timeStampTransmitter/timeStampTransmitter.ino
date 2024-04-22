// This script is intended forvhte arduino base station
// It includes the channel list each sonde is programed with
// Each sonde identity is matahced with the frequency varible name
// This script handles the frequency cycling, trasnmission of User inputs from a Serial input given by matlab
// And it prints any recieved data to the serial monitor which is opened in matlab.
#include <SPI.h>
#include <LoRa.h>

// unsigned long timeStamp;
// int timeStampArray[6];
String dataString;
// int x;
const long frequency = 910E6;  // LoRa Frequency
const long freq1 = 903000000;  // Channel 1
const long freq2 = 906000000;  // Channel 2
const long freq3 = 909000000;  // Channel 3
const long freq4 = 912000000;  // Channel 4
const long freq5 = 915000000;  // Channel 5
const long freq6 = 918000000;  // Channel 6

// This is the SPI frequency varialbe, it must be delcasred in the setup before the Lora.begin function
uint32_t spiFreq = 20;  // 10 MHz is the baseline

int packetSize; // packet size tells us how many bits exist in the Lora buffer

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("LoRa Receiver");

  LoRa.setSPIFrequency(spiFreq);  // base freq is 10 mHz
  // This can be initializied on any frequency due to the fact that this script is frequency cycling in the loop
  if (!LoRa.begin(freq2)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
}

void loop() {

// If a messaage is in the serial buffer this piece of logic will execute by trasnmitting the string, dataString
// Over the frequencies that are un commented in the statement.
  if (Serial.available()) {
    dataString = Serial.readString();
    LoRa.setFrequency(freq1);  // Channel 1
    LoRa.beginPacket();
    LoRa.print(dataString);
    LoRa.endPacket();

    LoRa.setFrequency(freq2);  // Channel 2
    LoRa.beginPacket();
    LoRa.print(dataString);
    LoRa.endPacket();

    LoRa.setFrequency(freq3);  // Channel 3
    LoRa.beginPacket();
    LoRa.print(dataString);
    LoRa.endPacket();

    // LoRa.setFrequency(freq4);  // Channel 4
    // LoRa.beginPacket();
    // LoRa.print(dataString);
    // LoRa.endPacket();

    // LoRa.setFrequency(freq5);  // Channel 5
    // LoRa.beginPacket();
    // LoRa.print(dataString);
    // LoRa.endPacket();

    // LoRa.setFrequency(freq6);  // Channel 6
    // LoRa.beginPacket();
    // LoRa.print(dataString);
    // LoRa.endPacket();
    Serial.flush(); // This hopefully flushes the serial buffer so duel transmissions dont occur
  }

  // Cycle between the frequencies the sondes are trasnmitting on 125 kHz minimum difference
  // The actual different is 3 MHz

  //////////////////////////////////////////////////////////////////////
  // Channel 1
  LoRa.setFrequency(freq1);
  // try to parse packet
  packetSize = LoRa.parsePacket();
  // If a packect exists packetSize is > 0 so the next piece of logic can execute
  if (packetSize) {
    // read packet
    if (LoRa.available()) {
      dataString = LoRa.readString();
      Serial.print(dataString);  // Bring displayed in matlab
    }
    //LoRa.flush();
  }

  /////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////
  // Channel 2
  LoRa.setFrequency(freq2);
  // try to parse packet
  packetSize = LoRa.parsePacket();
  // If a packect exists packetSize is > 0 so the next piece of logic can execute
  if (packetSize) {
    // read packet
    if (LoRa.available()) {
      dataString = LoRa.readString();
      Serial.print(dataString);  // Bring displayed in matlab
    }
    //LoRa.flush();
  }

  /////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////
  // Channel 3
  LoRa.setFrequency(freq3);
  // try to parse packet
  packetSize = LoRa.parsePacket();
  // If a packect exists packetSize is > 0 so the next piece of logic can execute
  if (packetSize) {
    // read packet
    if (LoRa.available()) {
      dataString = LoRa.readString();
      Serial.print(dataString);  // Bring displayed in matlab
    }
    //LoRa.flush();
  }

  /////////////////////////////////////////////////////////////////////////

  // ////////////////////////////////////////////////////////////////////
  // // Channel 4
  // LoRa.setFrequency(freq4);
  // // try to parse packet
  // packetSize = LoRa.parsePacket();
  // If a packect exists packetSize is > 0 so the next piece of logic can execute
  // if (packetSize) {
  //   // read packet
  //   if (LoRa.available()) {
  //     dataString = LoRa.readString();
  //     Serial.print(dataString);  // Bring displayed in matlab
  //   }
  // }

  // // ///////////////////////////////////////////////////////////////////////

  // //////////////////////////////////////////////////////////////////////
  // // Channel 5
  // LoRa.setFrequency(freq5);
  // // try to parse packet
  // packetSize = LoRa.parsePacket();
  // If a packect exists packetSize is > 0 so the next piece of logic can execute
  // if (packetSize) {
  //   // read packet
  //   if (LoRa.available()) {
  //     dataString = LoRa.readString();
  //     Serial.print(dataString);  // Bring displayed in matlab
  //   }
  // }

  // /////////////////////////////////////////////////////////////////////////

  // //////////////////////////////////////////////////////////////////////
  // // Channel 6
  // LoRa.setFrequency(freq6);
  // // try to parse packet
  // packetSize = LoRa.parsePacket();
  // If a packect exists packetSize is > 0 so the next piece of logic can execute
  // if (packetSize) {
  //   // read packet
  //   if (LoRa.available()) {
  //     dataString = LoRa.readString();
  //     Serial.print(dataString);  // Bring displayed in matlab
  //   }
  // }

  // /////////////////////////////////////////////////////////////////////////
}
