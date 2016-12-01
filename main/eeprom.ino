/**
 *7 days, 24 hours per day, 4 intervals per hour (15 min)
 *672 divisions per week, weekly schedules
 *schedule 0 : personal
 *schedule 1 : vacation
 *schedule 2 : powersaver
 *schedule 3 : machine learned schedule
 *
 *Schedule Format: Mon-Sun
 *                 Each day starts at 12:00am and ends 11:45PM
 */
#include "globalconstants.h"
#define EPS 1e-9

const int Eeprom = EEPROMADDR;

void EepromInit() { 
  float schedule[ScheduleArrayElements] = { 0 };

  //Input Vacation Schedule
  //16 C from 8pm to 7 am, 18 C from 7am to 8pm
  for (int i=0; i<DaysInWeek; i++) {
    for (int j=0; j<IntervalsInHour*7; j++) 
      schedule[i*IntervalsInHour*HoursInDay + j] = 16.0;
    for (int j=IntervalsInHour*7; j<IntervalsInHour*20; j++)
      schedule[i*IntervalsInHour*HoursInDay + j] = 18.0;
    for (int j=IntervalsInHour*20; j<IntervalsInHour*24; j++)
      schedule[i*IntervalsInHour*HoursInDay + j] = 16.0;
  }

  EepromWriteSchedule(schedule, 1);

  //Input Power Saver Schedule
  //21 C from 12am to 9 am, 18 C from 9am to 4pm, 21.5 C from 4pm to 12am
  for (int i=0; i<DaysInWeek; i++) {
    for (int j=0; j<IntervalsInHour*9; j++) 
      schedule[i*IntervalsInHour*HoursInDay + j] = 21.0;
    for (int j=IntervalsInHour*9; j<IntervalsInHour*16; j++)
      schedule[i*IntervalsInHour*HoursInDay + j] = 18.0;
    for (int j=IntervalsInHour*16; j<IntervalsInHour*24; j++)
      schedule[i*IntervalsInHour*HoursInDay + j] = 21.5;
  }

  EepromWriteSchedule(schedule, 2);
}

void EepromReadSchedule(float *readTo, int scheduleNum) {
  short memAddress = scheduleNum * ScheduleArraySize;
  
  for (int i=0; i<ScheduleArrayElements; i++) {
    delay(5);
    uint8_t mostSignificantBit    = (int)((memAddress+2*i) >> 8);
    uint8_t leastSignificantBit   = (int)((memAddress+2*i) & 0xFF);
    size_t const DataLength   = 1;
    uint32_t data[DataLength] = { 0 };
    
    WireWriteRegister(Eeprom, mostSignificantBit,  leastSignificantBit);
    WireRequestArray(Eeprom, data, DataLength);
    readTo[i] = data[0];
    if (readTo[i] > 128)
      readTo[i] -= 256;
    mostSignificantBit        = (int)((memAddress+2*i+1) >> 8);
    leastSignificantBit       = (int)((memAddress+2*i+1) & 0xFF);
    
    delay(5);
    WireWriteRegister(Eeprom, mostSignificantBit,  leastSignificantBit);
    WireRequestArray(Eeprom, data, DataLength);
    if (data[0] == 128)
      readTo[i] += 0.5;
  }
}

void EepromWriteSchedule(float *writeFrom, int scheduleNum) {
  short memAddress = scheduleNum * ScheduleArraySize;
  
  for (int i=0; i<ScheduleArrayElements; i++) {
    delay(5);
    int mostSignificantBit    = (int)((memAddress+2*i) >> 8);
    int leastSignificantBit   = (int)((memAddress+2*i) & 0xFF);
    uint8_t data              = 0;
    data = (int) (writeFrom[i] + EPS);
    uint8_t data2 = data;
    WireWriteByteToRegister(Eeprom, mostSignificantBit, leastSignificantBit, data);
    
    delay(5);
    mostSignificantBit        = (int)((memAddress+2*i+1) >> 8);
    leastSignificantBit       = (int)((memAddress+2*i+1) & 0xFF);
    if (fabs((writeFrom[i] - (int) (writeFrom[i] + EPS)) - 0.5) < EPS)
      data = 128;
    else 
      data = 0;
    WireWriteByteToRegister(Eeprom, mostSignificantBit, leastSignificantBit, data);
  }
}

void EepromReadByte(int *readByte, int memAddress) {
  delay(5);
  uint8_t mostSignificantBit    = (int)((memAddress) >> 8);
  uint8_t leastSignificantBit   = (int)((memAddress) & 0xFF);
  size_t const DataLength       = 1;
  uint32_t data[DataLength]     = { 0 };
 
  WireWriteRegister(Eeprom, mostSignificantBit,  leastSignificantBit);
  WireRequestArray(Eeprom, data, DataLength);
  *readByte = data[0];
}

void EepromWriteByte(int *writeByte, int memAddress) {
    delay(5);
    int mostSignificantBit    = (int)((memAddress) >> 8);
    int leastSignificantBit   = (int)((memAddress) & 0xFF);
    WireWriteByteToRegister(Eeprom, mostSignificantBit, leastSignificantBit, *writeByte);
}

