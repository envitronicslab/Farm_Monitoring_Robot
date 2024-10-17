#ifndef GPS_MODULE_H_
#define GPS_MODULE_H_

#include "manager.h"

class GPSModule {
public:
  GPSModule(Manager *src)
    : manager_(src) {}

  void Run();

private:
  void sendGPSData(const String &n1, const String &n2);

  Manager *manager_;

};      // class GPSModule
#endif  // GPS_MODULE_H_
