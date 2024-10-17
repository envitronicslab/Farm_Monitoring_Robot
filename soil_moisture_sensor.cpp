#include "soil_moisture_sensor.h"
#include <SPI.h>

// Link: https://learn.adafruit.com/adafruit-grand-central?view=all
SPISettings mySettting(500000, MSBFIRST, SPI_MODE0);
const uint8_t CS{ 7 };  // Sensor select pin

//*****************************************************************
// Create a thread that publishes soil moisture sensor data to the SPI bus
// This task will run forever
//*****************************************************************
void SoilMoistureSensor::Run() {
  manager_->SafePrintln("Soil moisture sensor task started");

  String sms{ "" };

  // SPI setup
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);  // Setting Chip Select as HIGH (So controller/M4 doesnt connnect with the sensor yet)
  SPI.begin();             // Begins the SPI commnuication

  while (1) {
    sms = manager_->getSMS();
    sendSoilMoistureSensorData(sms);

    manager_->myDelayMs(1000 / 250.f);  // 250 Hz
  }
}

void SoilMoistureSensor::sendSoilMoistureSensorData(const String &smsData) {
  digitalWrite(CS, LOW);
  delayMicroseconds(10);  // Give slave some time
  SPI.beginTransaction(mySettting);
  for (int i = 0; i < smsData.length(); i++) {
    SPI.transfer(smsData[i]);
    delayMicroseconds(10);
  }
  SPI.transfer('\n');
  delayMicroseconds(10);
  SPI.endTransaction();
  digitalWrite(CS, HIGH);
}
