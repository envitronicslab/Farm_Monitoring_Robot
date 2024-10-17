#ifndef ELECTRICAL_CONDUCTIVITY_H_
#define ELECTRICAL_CONDUCTIVITY_H_

#include "manager.h"


class ElectricalConductivitySensor {
public:
  ElectricalConductivitySensor(Manager *src)
    : manager_(src) {}

  void Run();

private:
  void sendECData(const String &ec_data);

  Manager *manager_;


};      // class ElectricalConductivitySensor
#endif  // ELECTRICAL_CONDUCTIVITY_H_
