#include "serial_com.h"


//*****************************************************************
// Create a thread that reads Serial port (USB)
// This task will run forever
//*****************************************************************
void SerialCom::Run() {
  manager_->SafePrintln("Serial data handling task started");

  while (1) {
    /* Read Serial */
    incommingData();
    makeSentences();

    manager_->myDelayMs(1000 / 250.f);  // 250 Hz
  }
}

void SerialCom::makeSentences() {
  /* 
    GPS data 
  */
  // http://aprs.gids.nl/nmea/
  // Format: $xxRMC,time,status,lat,NS,long,EW,spd,cog,date,mv,mvEW,posMode,navStatus*cs<CR><LF>
  /*
    String main = String("GPRMC," + manager_->getTimestamp() + ",A,4124.89,N,08151.68,W,000.0,360.0,130998,011.3,E"); // for debugging only 
    GPSData_GPRMC_ = String('$' + main + '*' + String(manager_->getCheckSum(main),HEX)); // for debugging only 
    GPSData_GPRMC_.toUpperCase(); // for debugging only  
    // Format: $xxGGA,time,lat,NS,long,EW,quality,numSV,HDOP,alt,M,sep,M,diffAge,diffStation*cs<CR><LF> 
    main = String("GPGGA," + manager_->getTimestamp() + ",4124.8963,N,08151.6838,W,2,05,1.2,280.2,M,-34.0,M,2.0,0031"); // for debugging only 
    GPSData_GPGGA_ = String('$' + main + '*' + String(manager_->getCheckSum(main),HEX)); // for debugging only 
    GPSData_GPGGA_.toUpperCase(); // for debugging only 
  */
  // Pass NMEA sentences to the GPS emulator thread
  manager_->setNmea1(GPSData_GPRMC_);
  manager_->setNmea2(GPSData_GPGGA_);

  /* 
    CO2 sensor data 
  */
  CO2DataBytes co2_byte;
  co2_byte.msb2 = byteMSB2(co2_);
  co2_byte.msb1 = byteMSB1(co2_);
  co2_byte.lsb = byteLSB(co2_);
  // Format data
  String main1 = String("CO2S,1,1," + String(co2_byte.msb2) + ',' + String(co2_byte.msb1) + ',' + String(co2_byte.lsb) + ',');
  String co2Data = String('$' + main1 + '*' + String(manager_->getCheckSum(main1), HEX));
  co2Data.toUpperCase();
  manager_->setCO2(co2Data);
  //  manager_->SafePrintln(co2Data);  // for debugging only

  /* 
    EC sensor data
  */
  //  ec = 0; // for debugging only
  //  temp_ec_ = 4648;  // for debugging only
  // Format data
  uint8_t ec_ecs[2];
  uint8_t temp_ecs[2];
  // EC
  ec_ecs[0] = byteMSB1(ec_ecs_);
  ec_ecs[1] = byteLSB(ec_ecs_);
  // Temperature
  temp_ecs[0] = byteMSB1(temp_ecs_);
  temp_ecs[1] = byteLSB(temp_ecs_);
  // Calculate the CRC
  uint8_t tempCRC = crc(&ec_ecs[0], 2);
  uint8_t ecCRC = crc(&temp_ecs[0], 2);
  // Format data
  String ec_str = String(String(ec_ecs[0]) + ',' + String(ec_ecs[1]) + ',' + String(tempCRC));
  String temp_ec_str = String(String(temp_ecs[0]) + ',' + String(temp_ecs[1]) + ',' + String(ecCRC));
  String main2 = String("ECS,1,1," + ec_str + ',' + temp_ec_str + ',');
  String ecData = String('$' + main2 + '*' + String(manager_->getCheckSum(main2), HEX));
  ecData.toUpperCase();
  manager_->setEC(ecData);
  //  manager_->SafePrintln(ecData);  // for debugging only

  /* 
    Soil moisture sensor data 
  */
  // Format data
  uint8_t moisture_sms[2];
  uint8_t temp_sms[2];
  // Gyroscope
  moisture_sms[0] = byteMSB1(moisture_sms_);
  moisture_sms[1] = byteLSB(moisture_sms_);
  // Temperature
  temp_sms[0] = byteMSB1(temp_sms_);
  temp_sms[1] = byteLSB(temp_sms_);
  // Format data
  String sms_str = String(String(moisture_sms[0]) + ',' + String(moisture_sms[1]));
  String temp_sms_str = String(String(temp_sms[0]) + ',' + String(temp_sms[1]));
  String main3 = String("SMS,1,1," + sms_str + ',' + temp_sms_str + ',');
  String smsData = String('$' + main3 + '*' + String(manager_->getCheckSum(main3), HEX));
  smsData.toUpperCase();
  manager_->setSMS(smsData);
  //  manager_->SafePrintln(smsData);  // for debugging only
}

/* 
  Calculate CRC8 for the sensor payload data 
*/
// array - > pointer (rule of thumb: any pointers passed into a function that will not be modified by the function must be done as const pointers)
uint8_t SerialCom::crc(const uint8_t data[], unsigned size) {
  uint8_t crc_value = 0xff;  // Initialization

  // Calculate 8-bit checksum with polynomial 0x31 (x^8 + x^5 + x^4 + 1)
  for (unsigned i = 0; i < size; i++) {
    // First binary XOR (^) copies the bit to the evaluated result if it is present in either of the operands but not both, and then assigns the result to the variable on the left 
    crc_value ^= (data[i]);

    for (int bit = 8; bit > 0; --bit) {
      if (crc_value & 0x80) {  // & copies a bit to the evaluated result if it exists in both operands
        crc_value = (crc_value << 1) ^ 0x31;

      } else {
        crc_value = (crc_value << 1);
      }
    }
  }

  // Return checksum
  return crc_value;
}

//############################################################################################
// -- READ SERIAL (PC COMMS) DATA --
//############################################################################################
void SerialCom::incommingData() {
  String strSerialMsg = "";
  char inByte1;

  if (manager_->TakeMSerial() == pdTRUE) {
    if (!Serial.available()) {
      manager_->GiveMSerial();
      return;
    } else {
      //        manager_->myDelayMs(5);
    }

    while (Serial.available() && serialMsgReady_ == false)  // If serial data is available, read in a single byte and add it to a String on each iteration
    {
      inByte1 = (char)Serial.read();  // get the new byte
      if (inByte1 == '\n') {          // if the incoming character is a newline, set a flag so the main loop can do something about it
        serialMsgReady_ = true;
        //          strSerialMsg.trim();
      } else {
        strSerialMsg += inByte1;  // add it to strSerialMsg
      }
    }

    manager_->GiveMSerial();
  }

  if (serialMsgReady_ == true) {
    const char chrLetter = strSerialMsg.charAt(0);
    const uint8_t bytMsgLength = strSerialMsg.length();
    if (chrLetter != '$') {
      // Reset variables
      strSerialMsg = "";
      serialMsgReady_ = false;

    } else {
      if (checksumCheck(strSerialMsg) == true) {
        getSentences(strSerialMsg);
        processSentences();
      }
      // Reset variables
      strSerialMsg = "";
      serialMsgReady_ = false;
    }
  }
}

boolean SerialCom::checksumCheck(const String &strSerialMsg) {
  const uint8_t bytLength = strSerialMsg.length();
  char chrLetter;
  uint8_t i, j;
  String str{ "" };
  String checkSum{ "" };

  uint16_t first = strSerialMsg.indexOf('$');
  uint16_t last = strSerialMsg.indexOf('*');
  str = strSerialMsg.substring(first + 1, last - first);
  checkSum = strSerialMsg.substring(last + 1, bytLength);

  //  manager_->SafePrintln(checkSum);   // for debugging only
  int checksum_int = (int)strtoll(&checkSum[0], NULL, 16);  // Provided checksum by the sender
  int checksum_cal = manager_->getCheckSum(str);            // Calculated checksum by M4

  if (checksum_int != 0 && str != "" && checksum_int == checksum_cal) {
    return true;
  } else {
    return false;
  }
}

void SerialCom::getSentences(const String &strSerialMsg) {
  const uint8_t bytLength = strSerialMsg.length();
  char chrLetter;
  uint8_t XA;
  uint8_t YA;

  for (XA = 0; XA < bytLength; XA++) {
    chrLetter = strSerialMsg.charAt(XA);
    if (chrLetter == '$') {
      for (YA = XA + 1; YA < bytLength; YA++) {
        chrLetter = strSerialMsg.charAt(YA);
        if (chrLetter == ',') {
          strCODE_ = strSerialMsg.substring(XA + 1, YA);
          break;
        }
      }
      break;
    }
  }
  DeviceNumber_ = parse(strSerialMsg, bytLength, ',', YA);
  Command_ = parse(strSerialMsg, bytLength, ',', Yj_);
  if (strCODE_ == "GPRMC" || strCODE_ == "GPGGA") {
    strGPS_Data_ = strSerialMsg;
  } else {
    for (int i = 0; i < 10; i++) {
      fltDistinguish[i] = parse(strSerialMsg, bytLength, ',', Yj_);
    }
  }
}

float SerialCom::parse(const String &strSerialMsg, const int bytLength, const char Letter, const uint8_t i) {
  uint8_t j;
  char chrLetter;
  float flt;
  for (j = i + 1; j < bytLength; j++) {
    chrLetter = strSerialMsg.charAt(j);
    if (chrLetter == Letter) {
      String str = strSerialMsg.substring(i + 1, j);
      flt = str.toFloat();
      break;
    }
  }
  Yj_ = j;
  return flt;
}

void SerialCom::processSentences() {
  // Example: $GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70
  if (strCODE_ == "GPRMC") {
    GPSData_GPRMC_ = strGPS_Data_;
    //    manager_->SafePrintln(GPSData_GPRMC_);   // for debugging only
    return;
  }

  // Example: $GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*HH
  if (strCODE_ == "GPGGA") {
    GPSData_GPGGA_ = strGPS_Data_;
    //    manager_->SafePrintln(GPSData_GPGGA_);   // for debugging only
    return;
  }

  // Format: $[Code],255,1,[CO2],*[Checksum]
  // Code: indicates what the data string is about (e.g. Code = CO2S indicates CO2 sensor data) 
  // CO2: indicates CO2 sensor data
  // Checksum: represents the number of bits in the transmitted message 
  // Example: $CO2S,255,1,0.6933,*xx
  if (strCODE_ == "CO2S") {
    co2_ = fltDistinguish[0] * SCALE_CO2;
    //    manager_->SafePrintln(String(co2_));   // for debugging only
    return;
  }

  // Format: $[Code],255,1,[ec],[temp],*[Checksum]
  // Code: indicates what the data string is about (e.g. Code = ECS indicates EC sensor data) 
  // ec: indicates electrical conductivity
  // temp: indicates temperature data
  // Checksum: represents the number of bits in the transmitted message 
  // Example: $ECS,255,1,0,23.24,*3F
  if (strCODE_ == "ECS") {
    ec_ecs_ = fltDistinguish[0] * SCALE_EC;             // Scale EC
    temp_ecs_ = fltDistinguish[1] * SCALE_TEMPERATURE;  // Scale temperature
                                                        //    manager_->SafePrintln(String(ec_));   // for debugging only
                                                        //    manager_->SafePrintln(String(temp_ec_));   // for debugging only
    return;
  }

  // Format: $[Code],255,1, [moisture], [temp],*[Checksum]
  // Code: indicates what the data string is about (e.g. Code = SMS indicates soil moisture sensor data) 
  // moisture: indicates soil moisture data
  // temp: indicates temperature
  // Checksum: represents the number of bits in the transmitted message 
  // Example: $SMS,255,1,67,23.24,*xx
  if (strCODE_ == "SMS") {
    // Soil moisture
    moisture_sms_ = fltDistinguish[5] * SCALE_SMS_MOISTURE;
    // Temperature
    temp_sms_ = (fltDistinguish[6] - OFFSET_SMS_TEMP) * SCALE_SMS_TEMP;
    //    manager_->SafePrintln(String(moisture_sms_));   // for debugging only
    //    manager_->SafePrintln(String(temp_sms_));   // for debugging only
    return;
  }
}
