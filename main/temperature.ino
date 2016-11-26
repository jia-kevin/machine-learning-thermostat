#include <stdbool.h>
#include <string.h>

const static int MinSetTemp        = 15;
const static int MaxSetTemp        = 40;
const static int NoSetTemp         = MinSetTemp - 1;
const static int TemperatureSensor = TEMPADDR;

static float   DesiredTemp;
static bool    HeatingOn;

void TempInit() {
  WireWriteRegister(TemperatureSensor, 0x20, 1);
  HeatingOn = false;
}

float TempRead() {
  size_t const DataLength = 6;
  uint32_t data[DataLength] = { 0 };
  
  WireWriteByte(TemperatureSensor, 0);
  WireRequestArray(TemperatureSensor, data, DataLength);

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
  return true;
}

int GetDesiredTemp() {
  return DesiredTemp;
}

void SetDesiredTemp(float desired) {
  DesiredTemp = desired;
}

int GetMinTemp() {
  return MinSetTemp;
}

int GetMaxTemp() {
  return MaxSetTemp;
}

int GetNoSetTemp() {
  return NoSetTemp;
}

void ControlHVAC() {
  
}


