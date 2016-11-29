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
#define EPS 1e-9

const static int Eeprom                 = EEPROMADDR;
const static int DaysInWeek             = 7;
const static int HoursInDay             = 24;
const static int IntervalsInHour        = 4;
const static int ScheduleArrayElements  = DaysInWeek * HoursInDay * IntervalsInHour;

void EepromInit() { 
  float schedule[ScheduleArrayElements] = { 0 };

  //Vacation Schedule
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

  //Power Saver Schedule
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
    int mostSignificantBit    = (memAddress+2*i) >> 8;
    int leastSignificantBit   = (memAddress+2*i) & 0xFF;
    size_t const DataLength   = 1;
    uint32_t data[DataLength] = { 0 };
    
    WireWriteRegister(Eeprom, mostSignificantBit,  leastSignificantBit);
    WireRequestArray(Eeprom, data, DataLength);
    readTo[i] = data[0];
    if (readTo[i] > 128)
      readTo[i] -= 256;

    mostSignificantBit        = (memAddress+2*i+1) >> 8;
    leastSignificantBit       = (memAddress+2*i+1) & 0xFF;
    
    WireWriteRegister(Eeprom, mostSignificantBit,  leastSignificantBit);
    WireRequestArray(Eeprom, data, DataLength);
    if (data[0] == 128)
      readTo[i] += 0.5;
  }
}

void EepromWriteSchedule(float *writeFrom, int scheduleNum) {
  short memAddress = scheduleNum * ScheduleArraySize;
  
  for (int i=0; i<ScheduleArrayElements; i++) {
    int mostSignificantBit    = (memAddress+2*i) >> 8;
    int leastSignificantBit   = (memAddress+2*i) & 0xFF;
    uint8_t data              = 0;
    
    data = (int) (WriteFrom[i] + EPS);
    WireWriteByteToRegister(Eeprom, mostSignificantBit, leastSignificantBit, data);

    mostSignificantBit        = (memAddress+2*i+1) >> 8;
    leastSignificantBit       = (memAddress+2*i+1) & 0xFF;
    
    if (fabs((writeFrom[i] - (int) (writeFrom[i] + EPS)) - 0.5) < EPS)
      data = 128;
    else 
      data = 0;
    WireWriteByteToRegister(Eeprom, mostSignificantBit, leastSignificantBit, data);
  }
}

