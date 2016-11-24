#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <string.h>

void setup() {
  Serial.begin(9600);
  OrbitOledInit();
  WireInit();
  TempInit();
  ControlInit();
}

void loop() {
  delay(100);
  DisplayMenu();
  ReadInput();
}

