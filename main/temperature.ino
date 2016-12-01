#include <stdbool.h>
#include <string.h>
#include <OrbitBoosterPackDefs.h>
#include "datetime.h"
#include "globalconstants.h"

const static int TemperatureSensor = TEMPADDR;

static float   DesiredTemp = GetNoSetTemp();
static float   FinalIntervalTemp = GetNoSetTemp(); 
static bool    HeatingOn;
static float   Schedules[NumModes-1][ScheduleArrayElements];

void TempInit() {
  LoadSchedules();
  WireWriteRegister(TemperatureSensor, 0x20, 1);
  HeatingOn = false;
}

float TempRead() {
  size_t const DataLength = 2;
  uint32_t data[DataLength] = { 0 };

  WireWriteByte(TemperatureSensor, 0);
  WireRequestArray(TemperatureSensor, data, DataLength);

  float temp = data[0];
  if (data[0] > 128)
    temp = temp - 256;
  if (data[1] == 128)
    temp += 0.5;

  if (GetTime().minute % MinutesInInterval == MinutesInInterval - 1) {
    if (GetDesiredTemp() == NoSetTemp)
      FinalIntervalTemp = temp;
    else
      FinalIntervalTemp = GetDesiredTemp();
  }
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

float GetFinalIntervalTemp() {
  return FinalIntervalTemp;
}

void LoadSchedules() {
  for (int i=0; i<NumModes-1; i++) {
    EepromReadSchedule(Schedules[i], i);
  }
}

void SaveNewSchedule(float newSchedule[], int mode) {
  EepromWriteSchedule(newSchedule, mode-1);
  for (int i=0; i<ScheduleArrayElements; i++) {
    Schedules[mode-1][i] = newSchedule[i];
  }
}

void ControlHVAC() {
  if (TempRead() < GetDesiredTemp())  {
    setHeater(true);
  }
  else {
    setHeater(false);
  }
}

void ControlTemp() {
  if (GetMode() && GetSwitchTempLock()) {
    struct DateTime current = GetTime();

    int index = 0;
    index += IntervalsInHour*HoursInDay*current.day;
    index += IntervalsInHour*current.hour;
    index += current.minute/MinutesInInterval;

    SetDesiredTemp(Schedules[GetMode()-1][index]);
  }

  ControlHVAC();
}

float GetScheduleTemp(int mode, int index) {
  return Schedules[mode-1][index];
}

