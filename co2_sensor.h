#ifndef CO2_SENSOR_H_
#define CO2_SENSOR_H_

#include "manager.h"

class CO2Sensor {
public:
  CO2Sensor(Manager *src)
    : manager_(src) {}

  void Run();

private:
  void sendCO2Data(const String &co2Data);

  Manager *manager_;

};      // class CO2Sensor
#endif  // CO2_SENSOR_H_
