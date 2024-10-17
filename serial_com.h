#ifndef SERIAL_COM_H_
#define SERIAL_COM_H_

#include "manager.h"

class SerialCom {
public:
  SerialCom(Manager *src)
    : manager_(src) {}

  void Run();

private:
  void makeSentences();
  void incommingData();
  bool checksumCheck(const String &strSerialMsg);
  void getSentences(const String &strSerialMsg);
  float parse(const String &strSerialMsg, int bytLength, char Letter, uint8_t i);  // const indicates these parameters will not be modified by the function
  void processSentences();
  uint8_t crc(const uint8_t data[], unsigned size);

  uint8_t byteMSB2(int data) {
    uint8_t dataByte = (data >> 16) & 0xFF;
    return dataByte;
  }

  uint8_t byteMSB1(int data) {
    uint8_t dataByte = (data >> 8) & 0xFF;
    return dataByte;
  }

  uint8_t byteLSB(int data) {
    uint8_t dataByte = data & 0xFF;
    return dataByte;
  }

  // Struct to hold CO2 data bytes
  struct CO2DataBytes {
    uint8_t msb2;
    uint8_t msb1;
    uint8_t lsb;
  };

  // Variables to hold sensor data
  static constexpr float SCALE_TEMPERATURE{ 200.0 };  // Temperature scale factor (1/°C)
  static constexpr float SCALE_EC{ 20.0f };           // EC scale factor (1/Pa)
  static constexpr float CONVERSION{ 100.0f };
  static constexpr float CO2_SENSITIVITY{ 75.0f };
  static constexpr float SCALE_CO2{ CO2_SENSITIVITY * CONVERSION };
  static constexpr float SCALE_SMS_MOISTURE{ 16.4f };
  static constexpr float SCALE_SMS_TEMP{ 333.87f };
  static constexpr float OFFSET_SMS_TEMP{ 21.0f };
  uint16_t co2_{ 0 };
  uint16_t ec_ecs_{ 0 };
  uint16_t temp_ecs_{ 0 };
  uint16_t moisture_sms_{ 0 };
  uint16_t temp_sms_{ 0 };
  String GPSData_GPRMC_{ "" };
  String GPSData_GPGGA_{ "" };
  String strGPS_Data_{ "" };

  // Variables to hold serial data
  bool serialMsgReady_{ false };
  uint8_t DeviceNumber_{ 0 };
  uint8_t Command_{ 0 };
  String strCODE_{ "" };
  float fltDistinguish[20];
  uint8_t bytLength_{ 0 };
  uint8_t Yj_{ 0 };

  Manager *manager_;

};      // class SerialCom
#endif  // SERIAL_COM_H_
