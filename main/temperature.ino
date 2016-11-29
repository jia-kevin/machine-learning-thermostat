#include <stdbool.h>
#include <string.h>
#include "datetime.h"

struct DateTime;

const        int MinSetTemp        = 15;
const        int MaxSetTemp        = 35;
const        int NoSetTemp         = MinSetTemp - 1;
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

float GetDesiredTemp() {
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
  if (TempRead() < GetDesiredTemp()) 
    setHeater(true);
  else
    setHeater(false);
}

void ControlTemp() {
  if (GetMode()) {
    float currentSchedule[ScheduleArrayElements];
    struct DateTime current = GetTime();
    
    EepromReadSchedule(currentSchedule, GetMode());

    int index = 0;
    index += IntervalsInHour*HoursInDay*current.day;
    index += IntervalsInHour*current.hour;
    index += current.minute/MinutesInInterval;

    SetDesiredTemp(currentSchedule[index]);
  }

  ControlHVAC();
}

