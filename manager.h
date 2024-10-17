#ifndef MANAGER_H_
#define MANAGER_H_

#include <FreeRTOS_SAMD51.h>


class Manager {
public:
  Manager() {}

  uint16_t getCheckSum(const String &servoFeedbackAll);

  void myDelayUs(int us) {
    vTaskDelay(us / portTICK_PERIOD_US);
  }

  void myDelayMs(int ms) {
    vTaskDelay((ms * 1000) / portTICK_PERIOD_US);
  }

  void myDelayMsUntil(TickType_t *previousWakeTime, int ms) {
    vTaskDelayUntil(previousWakeTime, (ms * 1000) / portTICK_PERIOD_US);
  }

  bool TakeM1(TickType_t ticks) {
    return xSemaphoreTake(m1_, ticks);
  }
  void GiveM1() {
    xSemaphoreGive(m1_);
  }

  bool TakeM2(TickType_t ticks) {
    return xSemaphoreTake(m2_, ticks);
  }
  void GiveM2() {
    xSemaphoreGive(m2_);
  }

  bool TakeM3(TickType_t ticks) {
    return xSemaphoreTake(m3_, ticks);
  }
  void GiveM3() {
    xSemaphoreGive(m3_);
  }

  bool TakeM4(TickType_t ticks) {
    return xSemaphoreTake(m4_, ticks);
  }
  void GiveM4() {
    xSemaphoreGive(m4_);
  }

  bool TakeM5(TickType_t ticks) {
    return xSemaphoreTake(m5_, ticks);
  }
  void GiveM5() {
    xSemaphoreGive(m5_);
  }

  bool TakeM6(TickType_t ticks) {
    return xSemaphoreTake(m6_, ticks);
  }
  void GiveM6() {
    xSemaphoreGive(m6_);
  }

  bool TakeM7(TickType_t ticks) {
    return xSemaphoreTake(m7_, ticks);
  }
  void GiveM7() {
    xSemaphoreGive(m7_);
  }

  bool TakeMSerial(TickType_t ticks = kDefaultTicks_) {
    return TakeWithCheck(mSerial_);
  }
  void GiveMSerial() {
    GiveWithCheck(mSerial_);
  }

  void setTimestamp(const String &timestamp) {
    if (TakeM7((TickType_t)10) == pdTRUE) {
      strTimeStamp_ = timestamp;
      GiveM7();
    }
  }

  void setNmea1(const String &nmea) {
    if (TakeM1((TickType_t)10) == pdTRUE) {
      nmea1_ = nmea;
      GiveM1();
    }
  }

  void setNmea2(const String &nmea) {
    if (TakeM1((TickType_t)10) == pdTRUE) {
      nmea2_ = nmea;
      GiveM1();
    }
  }

  void setCO2(const String &co2) {
    if (TakeM2((TickType_t)10) == pdTRUE) {
      co2_ = co2;
      GiveM2();
    }
  }

  void setEC(const String &ec) {
    if (TakeM3((TickType_t)10) == pdTRUE) {
      ec_ = ec;
      GiveM3();
    }
  }

  void setSMS(const String &sms) {
    if (TakeM4((TickType_t)10) == pdTRUE) {
      sms_ = sms;
      GiveM4();
    }
  }

  String getNmea1() {
    String cp{ "" };
    if (TakeM1((TickType_t)10) == pdTRUE) {
      cp = nmea1_;
      GiveM1();
    }
    return cp;
  }

  String getNmea2() {
    String cp{ "" };
    if (TakeM1((TickType_t)10) == pdTRUE) {
      cp = nmea2_;
      GiveM1();
    }
    return cp;
  }

  String getCO2() {
    String cp{ "" };
    if (TakeM2((TickType_t)10) == pdTRUE) {
      cp = co2_;
      GiveM2();
    }
    return cp;
  }

  String getEC() {
    String cp{ "" };
    if (TakeM3((TickType_t)10) == pdTRUE) {
      cp = ec_;
      GiveM3();
    }
    return cp;
  }

  String getSMS() {
    String cp{ "" };
    if (TakeM4((TickType_t)10) == pdTRUE) {
      cp = sms_;
      GiveM4();
    }
    return cp;
  }

  String getTimestamp() {
    String cp{ "" };
    if (TakeM7((TickType_t)10) == pdTRUE) {
      cp = strTimeStamp_;
      GiveM7();
    }
    return cp;
  }

  bool TakeWithCheck(SemaphoreHandle_t sem, TickType_t ticks = kDefaultTicks_) {
    if (sem == NULL) return pdFALSE;
    else return xSemaphoreTake(sem, ticks);
  }

  void GiveWithCheck(SemaphoreHandle_t sem) {
    if (sem == NULL) return;
    xSemaphoreGive(sem);
  }

  void SafePrintln(const String &message) {
    if (TakeWithCheck(mSerial_) == pdTRUE) {
      Serial.println(message);
      GiveWithCheck(mSerial_);
    }
  }

  void SafePrint(const String &message) {
    if (TakeWithCheck(mSerial_) == pdTRUE) {
      Serial.print(message);
      GiveWithCheck(mSerial_);
    }
  }

  static constexpr uint16_t timeDelay{ 500 };

private:
  static constexpr TickType_t kDefaultTicks_ = 10;
  /* https://www.freertos.org/xSemaphoreCreateMutexStatic.html
    https://www.digikey.com/en/maker/projects/introduction-to-rtos-solution-to-part-7-freertos-semaphore-example/51aa8660524c4daba38cba7c2f5baba7
    where a mutex implies a level of ownership of the lock (i.e. a single thread is said to have the lock while executing in a critical section), 
    a semaphore is a counter that can allow multiple threads to enter a critical section.
    */
  SemaphoreHandle_t m1_{ xSemaphoreCreateMutex() };
  SemaphoreHandle_t m2_{ xSemaphoreCreateMutex() };
  SemaphoreHandle_t m3_{ xSemaphoreCreateMutex() };
  SemaphoreHandle_t m4_{ xSemaphoreCreateMutex() };
  SemaphoreHandle_t m5_{ xSemaphoreCreateMutex() };
  SemaphoreHandle_t m6_{ xSemaphoreCreateMutex() };
  SemaphoreHandle_t m7_{ xSemaphoreCreateMutex() };
  SemaphoreHandle_t mSerial_{ xSemaphoreCreateMutex() };

  String nmea1_{ "" };
  String nmea2_{ "" };
  String co2_{ "" };
  String ec_{ "" };
  String sms_{ "" };
  String strTimeStamp_{ "" };

};      // class Manager
#endif  // MANAGER_H_
