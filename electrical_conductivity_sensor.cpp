#include "electrical_conductivity_sensor.h"
#include "sercomlib.h"

sercom_UseSerial(5, 4)  // Serial4 - pins 14(TX) and 15(RX) >> UART

//*****************************************************************
// Create a thread that publishes electrical conductivity sensor data to Serial4
// This task will run forever
//*****************************************************************
void ElectricalConductivitySensor::Run() {
  manager_->SafePrintln("EC sensor task started.");

  String ec{ "" };

  Serial4.begin(256000);  // Serial4 (pins 14 and 15) already defined.

  while (1) {
    ec = manager_->getEC();
    sendECData(ec);

    manager_->myDelayMs(1000 / 20.f);  // 20 Hz
  }
}


void ElectricalConductivitySensor::sendECData(const String &ec_data) {
  Serial4.println(ec_data);
}
