#ifndef SERVO_CONTROL_H_
#define SERVO_CONTROL_H_

#include "manager.h"

class ServoControl {
public:
  ServoControl(Manager *src)
    : manager_(src) {}

  void Run();
  void readPosFeedback(uint8_t feedbackPin);
  void printPosFeedback();

private:
  uint8_t numOfServos_{ 14 };
  int16_t analogFeedback_[14];
  uint16_t analogPin_[14]{ A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13 };
  uint16_t CALIB_MAX_[14]{ 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024 };
  uint16_t CALIB_MIN_[14]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  String str_Code_{ "SERVO" };

  Manager *manager_;

};      // class ServoControl
#endif  // SERVO_CONTROL_H_
