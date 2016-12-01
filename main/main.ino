#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <string.h>
#include "globalconstants.h"
#include "datetime.h"

void setup() {
  Serial.begin(9600);
  OrbitOledInit();
  char outputLine[ScreenWidth+1] = "Loading...";
  OrbitOledClear();
  OrbitOledSetCursor(0, 0);
  OrbitOledPutString(CenterLine(outputLine));
  
  WireInit();
  EepromInit();
  TempInit();
  ControlInit();
  RLInit();
  UiInit();
  TimeInit();
}

void loop() {
  delay(100);
  ReadInput();
  DisplayTick();
  ControlTemp();
  if (GetIsMachineLearning()) 
    PollMLData();
  if (GetTime().day == 0 && GetTime().hour == 2 && GetTime().minute == 1) //Monday, 2:01 AM
    LoadSchedules();                                                      //Update machine learning schedules once a week
}
