#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <string.h>

void WireInit();
void TempInit();
void ControlInit();

void  DisplayTemp();
void  ReadInput();
void setup() {
  Serial.begin(9600);
  OrbitOledInit();
  WireInit();
  TempInit();
  ControlInit();
}

void loop() {
  delay(5000);
  DisplayTemp();
  ReadInput();
}

