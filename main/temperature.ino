#include <stdbool.h>
#include <string.h>
#include <OrbitBoosterPackDefs.h>
#include "datetime.h"
#include "globalconstants.h"

const static int TemperatureSensor = TEMPADDR;

static float   DesiredTemp = GetNoSetTemp();
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

void LoadSchedules() {
  for (int i=0; i<NumModes-1; i++) {
    EepromReadSchedule(Schedules[i], i);
  }
}

void ControlHVAC() {
  Serial.println("helloooo");
  char output[20];
  sprintf(output, "%g", GetDesiredTemp());
  Serial.println(output);
  sprintf(output, "%g", TempRead());
  Serial.println(output);
  if (TempRead() < GetDesiredTemp())  {
  Serial.println("helloooo1");
    setHeater(true);
  Serial.println("helloooo2");
  }
  else {
  Serial.println("helloooo3");
    setHeater(false);
  Serial.println("helloooo4");
  }
}

void ControlTempMenu() {
  
}
void ControlTemp() {
  if (GetMode() && GetSwitchTempLock()) {
    Serial.println("1 ");
    Serial.println("22");
    Serial.println(" ");
    struct DateTime current = GetTime();

    int index = 0;
    index += IntervalsInHour*HoursInDay*current.day;
    index += IntervalsInHour*current.hour;
    index += current.minute/MinutesInInterval;

    SetDesiredTemp(Schedules[GetMode()][index]);
  }

  ControlHVAC();
}

