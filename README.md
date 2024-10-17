## Farm Monitoring Robot: FreeRTOS-based Framework

This repository provides a FreeRTOS-based embedded software framework for a farm monitoring robot. It's designed to run on an ARM Cortex-M4 microcontroller, such as the Adafruit Feather M4 (link: [https://www.adafruit.com/product/4064](https://www.adafruit.com/product/4064)).

**Key Features:**

* Multi-threaded architecture for efficient task management.
* **Sensor Data Handling:**
    * Serial data reception and parsing from sensors (soil moisture, EC, temperature).
    * I2C communication emulation for CO2 and EC sensors.
    * SPI communication emulation for soil moisture sensor connected to Raspberry Pi Pico.
* **Servo Feedback Monitoring:**
    * Reads position data from up to 12 servos connected to the M4 analog pins.
    * Formats feedback as NMEA-like sentences.
* **GPS Module Emulation:**
    * Simulates a GPS module, sending NMEA sentences (GPRMC & GPGGA) over UART.

**Important Notes:**

* This framework uses emulated sensors for development purposes. Replace them with actual environmental sensors.
* The code provides a foundation for building a complete monitoring system. You can easily add support for additional sensors (e.g. IMU, magnetometer) as separate tasks.

**Getting Started**

1. **Hardware Requirements:**
    * ARM Cortex-M4 microcontroller (e.g., Adafruit Feather M4)
    * Raspberry Pi Pico (optional, for SPI communication emulation)
    * Environmental sensors (soil moisture, EC, CO2) and servos (optional for real-world application)

2. **Software Requirements:**
    * Arduino IDE

3. **Building the Project:**
    * Refer to the specific documentation for your chosen development environment.

**Installation Dependencies**

While specific library requirements may depend on your development environment, you can find general instructions on installing libraries in the Arduino IDE here: [https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries)
