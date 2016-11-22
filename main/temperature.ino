#include <stdbool.h>
#include <string.h>

const static int MinSetTemp = 15;
const static int MaxSetTemp = 40;

static uint8_t DesiredTemp;
static bool    ControlTemp;
static bool    HeatingOn;

void TempInit() {
  WireWriteRegister(TEMPADDR, 0x20, 1);
  HeatingOn = false;
  ControlTemp = false;
}

float TempRead() {
  size_t const DataLength = 6;
  uint32_t data[DataLength] = { 0 };
  
  WireWriteByte(TEMPADDR, 0);
  WireRequestArray(TEMPADDR, data, DataLength);

  float temp = data[0];
  if (data[0] > 128) 
    temp = temp - 256;
  if (data[1] == 128) 
    temp += 0.5;
  
  return temp;
}

bool TempIsSet() {
  if (DesiredTemp < MinSetTemp || DesiredTemp > MaxSetTemp) 
    return false;

  return ControlTemp;
}

int GetDesiredTemp() {
  return DesiredTemp;
}



