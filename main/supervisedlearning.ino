/*
 * Data Collection and Algorithm based on Supervised Learning (type of Machine Learning)
 * 
 * Collects:
 *  # of times each interval was visited
 *  # of times temp was set in the interval
 *  actual temp of each interval
 *  if temp was set during previous period
 *  
 * Output:
 *  modified machine learned schedule
 *  based on factors above
 *  supervised learning uses reward method
 *  we calculate the reward with the data
 *  the reward for an impactful new temp is big
 *  
 *  the reward will be a scalar between 0 and 1
 *  and be used to created a weighted average between 
 *  old data and new data (probabilistic approach with 
 *  generally newer data being more important)
 */

#include <math.h>
#include "globalconstants.h"
#include "datetime.h"
#define EPS 1e-9

//2 check-bytes to see if supervisedlearning has been initialized before
const int ConfirmMLInitAddress1 = 30000;
const int ConfirmMLByteValue1   = 123;
const int ConfirmMLInitAddress2 = 30000;
const int ConfirmMLByteValue2   = 52;

//address where data is stored in eeprom
const int ScheduleMLAddress     = ScheduleArraySize*3;          
const int IterationsMLAddress   = 10000;
const int TimesChangedMLAddress = 15000;

const int UnchangedPenalty = 0.4; //reduce reward if temp not changed
const int MaxWeight        = 0.6; //Maximum reward

int  prevMin     = 0;
bool tempChanged = false;

void SLInit() {
  int check[1];
  EepromReadByte(check, ConfirmMLInitAddress1);
  if (check[0] == ConfirmMLByteValue1) return;
  
  EepromReadByte(check, ConfirmMLInitAddress2);
  if (check[0] == ConfirmMLByteValue2) return;

  int send[1];
  send[0] = ConfirmMLByteValue1; 
  EepromWriteByte(send, ConfirmMLInitAddress1);
  
  send[0] = ConfirmMLByteValue2; 
  EepromWriteByte(send, ConfirmMLInitAddress2);
  
  send[0] = 0;
  for (int i=0; i<ScheduleArrayElements; i++)
    EepromWriteByte(send, IterationsMLAddress + i);
  for (int i=0; i<ScheduleArrayElements; i++)
    EepromWriteByte(send, TimesChangedMLAddress + i);

  float emptySchedule[ScheduleArraySize];
  for (int i=0; i<ScheduleArraySize; i++)
    emptySchedule[i] = GetNoSetTemp();
   
  EepromWriteSchedule(emptySchedule, 3);
}

void PollMLData() {
  //only poll and use ml algorithm if a new piece of data is recieved (ie an interval is over)
  if (!((prevMin%MinutesInInterval == MinutesInInterval-1) && (GetTime().minute%MinutesInInterval == 0))) {
    prevMin = GetTime().minute;
    return;
  }

  int indexToChange = (GetTime().day*HoursInDay*IntervalsInHour + 
                       GetTime().hour*IntervalsInHour + 
                       GetTime().minute/MinutesInInterval 
                       - 1 + ScheduleArrayElements)%ScheduleArrayElements;

  int readB[1]; 
  EepromReadByte(readB, IterationsMLAddress + indexToChange);

  //if this is the first time receieving temp data for this time, set ml schedule to set temp
  if (readB[0] == 0) {
    int writeB[1];
    writeB[0] = 1;
    EepromWriteByte(writeB, IterationsMLAddress + indexToChange);

    if (tempChanged)
      EepromWriteByte(writeB, TimesChangedMLAddress + indexToChange);
    
    int data1[1];
    data1[0] = (int) (GetFinalIntervalTemp() + EPS);
    
    int data2[1];
    data2[0] = ((fabs)(GetFinalIntervalTemp() - data1[0] - 0.5) < EPS) ? 128 : 0;
    if (data1[0] < 0) 
      data1[0] += 256;
    EepromWriteByte(data1, ScheduleArraySize*3 + 2*indexToChange);
    EepromWriteByte(data2, ScheduleArraySize*3 + 2*indexToChange);
  }
  //else perform algorithm and produce new ml schedule
  else {
    int prevIter = readB[0];
    
    EepromReadByte(readB, TimesChangedMLAddress + indexToChange);
    int prevChanged = readB[0];

    float basePenalty = fabs(GetScheduleTemp(4, indexToChange) - GetFinalIntervalTemp());
    float finalReward  = (100 - basePenalty)/100;

    finalReward /= pow(prevIter, 0.5);
    
    if (tempChanged)
      finalReward *= pow((prevIter + 1)/(prevChanged+1), 0.5);
    else 
      finalReward *= UnchangedPenalty;

    if (finalReward > MaxWeight)
      finalReward = MaxWeight;

    float newTemp;
    newTemp = (GetFinalIntervalTemp() * finalReward) + (GetScheduleTemp(4, indexToChange) * (1 - finalReward));
    
    int data1[1];
    data1[0] = (int) (newTemp + EPS);
    int data2[1];
    data2[0] = ((newTemp - data1[0]) > 0.5) ? 128 : 0;if (data1[0] < 0) 
    data1[0] += 256;
    EepromWriteByte(data1, ScheduleArraySize*3 + 2*indexToChange);
    EepromWriteByte(data2, ScheduleArraySize*3 + 2*indexToChange);

    int writeB[1];
    writeB[0] = prevIter + 1;
    EepromWriteByte(writeB, IterationsMLAddress + indexToChange);
    
    if (tempChanged) {
      writeB[0] = prevChanged + 1;
      EepromWriteByte(writeB, TimesChangedMLAddress + indexToChange);
    }
  }

  prevMin     = GetTime().minute;
  tempChanged = false;
}

void TempChangeToTrue() {
  tempChanged = true;
}

