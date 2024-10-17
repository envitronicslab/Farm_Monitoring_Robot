#include "manager.h"


uint16_t Manager::getCheckSum(const String &data) {
  // Online calculator for comparison: https://nmeachecksum.eqth.net/
  char checkSum = 0;
  // Iterate over the string, XOR each byte with the total sum:
  for (int c = 0; c < data.length(); c++) {
    checkSum = char(checkSum ^ data.charAt(c));
  }
  // Return the checksum
  return checkSum;
}
