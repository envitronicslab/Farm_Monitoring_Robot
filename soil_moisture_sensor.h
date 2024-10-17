#ifndef SOIL_MOISTURE_SENSOR_H_
#define SOIL_MOISTURE_SENSOR_H_

#include "manager.h"

class SoilMoistureSensor {
public:
  SoilMoistureSensor(Manager *src)
    : manager_(src) {}

  void Run();

private:
  void sendSoilMoistureSensorData(const String &SoilMoistureSensorData);

  Manager *manager_;

};      // class SoilMoistureSensor
#endif  // SOIL_MOISTURE_SENSOR_H_
