#include "servo_control.h"

//*****************************************************************
// Create a thread that sends Servo Feedbacks to Controler
// This task will run forever
//*****************************************************************
void ServoControl::Run() {
  manager_->SafePrintln("Servo feedback monitoring task started");

  while (1) {
    // Read position feedback from all servos
    for (int feedbackPin = 0; feedbackPin < numOfServos_; feedbackPin++) {
      readPosFeedback(feedbackPin);
    }
    // Print all in one string of data
    printPosFeedback();
    manager_->myDelayMs(1000 / 100.f);  // 100 Hz
  }
}

void ServoControl::readPosFeedback(uint8_t feedbackPin) {
  pinMode(feedbackPin, INPUT);
  int16_t a{ analogRead(analogPin_[feedbackPin]) };
  // Calibrate and map to 0-180 degrees
  /*if (a < CALIB_MIN_[feedbackPin]) a = CALIB_MIN[feedbackPin];
  if (a > CALIB_MAX_[feedbackPin]) a = CALIB_MAX[feedbackPin];
  // int <newvalue> = map(<value>, <original_min>, <original_max>, <new_min>, <new_max>); 
  // Link: https://flaviocopes.com/arduino-language-map/
  analogFeedback_[feedbackPin] = map(a, CALIB_MIN[feedbackPin], CALIB_MAX[feedbackPin], 0, 254);*/

  /*// Map to -1x1000 to +1x1000 range. Values normally change from 0 to 1023 (10 bit resolution)  
  analogFeedback_[feedbackPin] = map(a, 0, 1023, -1000, 1000);*/

  analogFeedback_[feedbackPin] = a;
}

void ServoControl::printPosFeedback() {
  int intCheckSum { 0 };
  String str_analogFeedback[numOfServos_];

  for (int i = 0; i < numOfServos_; i++) {
    str_analogFeedback[i] = String(analogFeedback_[i]);
  }

  String servoFeedbackAll{ "" };
  servoFeedbackAll = String(str_Code_ + ',');  // concatenating two strings
  for (int j = 0; j < (numOfServos_ - 1); j++) {
    servoFeedbackAll = String(servoFeedbackAll + str_analogFeedback[j] + ',');
  }
  servoFeedbackAll = String(servoFeedbackAll + str_analogFeedback[numOfServos_ - 1]);
  intCheckSum = manager_->getCheckSum(servoFeedbackAll);
  servoFeedbackAll = String('$' + servoFeedbackAll + '*' + String(intCheckSum, HEX));
  servoFeedbackAll.toUpperCase();

  manager_->SafePrintln(servoFeedbackAll);
}
