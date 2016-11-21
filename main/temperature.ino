#include "twoWire.h"
#include "temperature.h"

void TempInit() {
  WireWriteRegister(TEMPADDR, 0x20, 1);
}

float TempRead() {
  size_t const DataLength = 6;
  uint32_t data[DataLength] = { 0 };
  WireWriteByte(TEMPADDR, 0);
  WireRequestArray(TEMPADDR, data, DataLength);
  float temp = data[0];
  if (data[0] > 128) temp = temp - 256;
  if (data[1]) temp += 0.5;
  
  return temp;
}

