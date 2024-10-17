//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// Written by Y. Osroosh, Ph.D.
// Email: yosroosh@gmail.com
//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#include "soil_moisture_sensor.h"
#include "electrical_conductivity_sensor.h"
#include "gps_module.h"
#include "co2_sensor.h"
#include "manager.h"
#include "serial_com.h"
#include "servo_control.h"


static constexpr uint8_t ledPin{ 13 };  // constexpr roughly means "must be known at compile time"
//  static constexpr uint8_t ledPin{ 2 }; // Led Pin: samd21 xplained board
static constexpr boolean ERROR_LED_LIGHTUP_STATE{ LOW };  // the state that makes the led light up on your board, either low or high


//*****************************************************************
void setup() {
  delay(1000);  // Prevents crash on startup, do not omit this

  Serial.println("");
  Serial.println("******************************");
  Serial.println("        Program start         ");
  Serial.println("******************************");

  /*
      Set the led the RTOS will blink when we have a fatal rtos error
      RTOS also Needs to know if high/low is the state that turns on the led.
      Error Blink Codes:
          3 blinks - Fatal RTOS Error, something bad happened. Think really hard about what you just changed.
          2 blinks - Malloc Failed, Happens when you couldn't create a RTOS object. 
                    Probably ran out of heap.
          1 blink  - Stack overflow, Task needs more bytes defined for its stack! 
                    Use the taskMonitor thread to help gauge how much more you need
    */
  pinMode(ledPin, OUTPUT);
  vSetErrorLed(ledPin, ERROR_LED_LIGHTUP_STATE);

  /*
      Create the threads that will be managed by the RTOS
      Sets the stack size and priority of each task
      Also initializes a handler pointer to each task, which are important to communicate with and retrieve info from tasks
    */
  Manager *manager = new Manager();
  GPSModule *gps_module = new GPSModule(manager);
  SerialCom *serial_com = new SerialCom(manager);
  CO2Sensor *co2_sensor = new CO2Sensor(manager);
  ElectricalConductivitySensor *ec_sensor = new ElectricalConductivitySensor(manager);
  SoilMoistureSensor *soil_moisture_sensor = new SoilMoistureSensor(manager);
  ServoControl *servo_control = new ServoControl(manager);

  /* The scheduling policy of the RTOS is to simply ensure that the highest priority task that is able to execute is the task given processing time. 
     This may require sharing processing time "fairly" between tasks of equal priority if they are ready to run simultaneously.  
    
    Implementation:   https://www.freertos.org/implementation/a00007.html
    Task priorities:  https://www.freertos.org/RTOS-task-priority.html
    */
  xTaskCreate(
    RunGPSModule,
    "Gps Module",
    256,
    gps_module,
    tskIDLE_PRIORITY + 1,
    NULL);

  xTaskCreate(
    RunSerialCom,
    "Serial Communication",
    512,
    serial_com,
    tskIDLE_PRIORITY + 1,
    NULL);

  xTaskCreate(
    RunCO2Sensor,
    "CO2 Sensor",
    256,
    co2_sensor,
    tskIDLE_PRIORITY + 1,
    NULL);

  xTaskCreate(
    RunECSensor,
    "EC Sensor",
    256,
    ec_sensor,
    tskIDLE_PRIORITY + 1,
    NULL);

  xTaskCreate(
    RunSoilMoistureSensor,
    "Soil Moisture Sensor",
    256,
    soil_moisture_sensor,
    tskIDLE_PRIORITY + 1,
    NULL);

  xTaskCreate(
    RunServoControl,
    "Servo Control",
    256,
    servo_control,
    tskIDLE_PRIORITY + 1,
    NULL);

  // Start the RTOS, this function will never return and will schedule the tasks.
  vTaskStartScheduler();

  // Error scheduler failed to start
  while (1) {
    Serial.println("Scheduler Failed! \n");
    delay(1000);
  }
}

void loop() {
  /* 
      This is now the RTOS idle loop
      No RTOS blocking functions allowed!
    */

  // Optional commands, can comment/uncomment below
  digitalWrite(ledPin, HIGH);
  delay(500);

  digitalWrite(ledPin, LOW);
  delay(500);
}

void RunGPSModule(void *params) {
  static_cast<GPSModule *>(params)->Run();
}

void RunSerialCom(void *params) {
  static_cast<SerialCom *>(params)->Run();
}

void RunCO2Sensor(void *params) {
  static_cast<CO2Sensor *>(params)->Run();
}

void RunECSensor(void *params) {
  static_cast<ElectricalConductivitySensor *>(params)->Run();
}

void RunSoilMoistureSensor(void *params) {
  static_cast<SoilMoistureSensor *>(params)->Run();
}

void RunServoControl(void *params) {
  static_cast<ServoControl *>(params)->Run();
}
