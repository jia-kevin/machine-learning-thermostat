#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <string.h>
#include "temperature.h"
#include "twoWire.h"

void setup() {
  Serial.begin(9600);
  OrbitOledInit();
  WireInit();
  TempInit();
}

void loop() {
  delay(5000);
  OrbitOledClear();
  OrbitOledSetCursor(1, 0);
  char tempOutput[20] = "";
  sprintf(tempOutput, "Temp is: %g C", TempRead());
  OrbitOledPutString(tempOutput);
}

