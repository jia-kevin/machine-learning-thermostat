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
}

void loop() {
  delay(5000);
  OrbitOledClear();
  OrbitOledSetCursor(1, 0);
  char tempOutput[20] = "";
  sprintf(tempOutput, "Temp is: %g", TempRead());
  OrbitOledPutString(tempOutput);
}

void TempInit() {
  //WireWriteRegister(TEMPADDR, 0x31, 1);
  WireWriteRegister(TEMPADDR, 0x20, 1);
}

float TempRead() {
  size_t const DataLength = 6;
  uint32_t data[DataLength] = { 0 };
  WireWriteByte(TEMPADDR, 0);
  WireRequestArray(TEMPADDR, data, DataLength);
  float temp = data[0];
  if (data[0] > 128) temp = temp - 256;
  if (data[1]) temp += 0.5;
  
  return temp;
}

