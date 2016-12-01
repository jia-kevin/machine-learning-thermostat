#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <string.h>
#include "globalconstants.h"

void setup() {
  Serial.begin(9600);
  OrbitOledInit();
  char outputLine[ScreenWidth+1] = "Loading...";
  OrbitOledClear();
  OrbitOledSetCursor(0, 0);
  OrbitOledPutString(CenterLine(outputLine));
  
  WireInit();
  //EepromInit();
  TempInit();
  ControlInit();
  UiInit();
  TimeInit();
}

void loop() {
  delay(100);
  Serial.println("1");
  ReadInput();
  Serial.println("12");
  DisplayTick();
  Serial.println("123");
  ControlTemp();
  Serial.println("1234");
}
