#include "gps_module.h"
#include "sercomlib.h"

sercom_UseSerial(4, 2)  // Serial2 - pins 18(TX) and 19(RX) >> UART

  //*****************************************************************
  // Create a thread that prints out GPS data to Serial2
  // This task will run forever
  //*****************************************************************
  void GPSModule::Run() {
  manager_->SafePrintln("GPS module task started.");

  String n1{ "" };
  String n2{ "" };

  Serial2.begin(38400);

  while (1) {
    n1 = manager_->getNmea1();
    n2 = manager_->getNmea2();
    sendGPSData(n1, n2);
    manager_->myDelayMs(1000 / 20.f);  // 20 Hz
  }
}

void GPSModule::sendGPSData(const String &nmea1, const String &nmea2) {
  // Send a GPGGA NMEA sentence
  Serial2.println(nmea1);
  //    manager_->SafePrintln(nmea1);   // for debugging only

  // Send a GPRMC NMEA sentence
  Serial2.println(nmea2);
  //    manager_->SafePrintln(nmea2);   // for debugging only
}
