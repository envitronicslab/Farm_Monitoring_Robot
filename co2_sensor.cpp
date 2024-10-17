#include "co2_sensor.h"
#include "sercomlib.h"

sercom_UseSerial(1, 3)  // Serial3 - pins 16(TX) and 17(RX) >> UART

//*****************************************************************
// Create a thread that publishes CO2 sensor data to Serial3
// This task will run forever
//*****************************************************************
void CO2Sensor::Run() {
  manager_->SafePrintln("CO2 sensor task started");

  String co2{ "" };

  Serial3.begin(256000);

  while (1) {
    co2 = manager_->getCO2();
    sendCO2Data(co2);

    manager_->myDelayMs(1000 / 75.f);  // 75 Hz
  }
}

void CO2Sensor::sendCO2Data(const String &CO2Data) {
  Serial3.println(CO2Data);
}
