#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <string.h>
#include <stdbool.h>

//static enum Pages

void DisplayMenu() {
}

void DisplayTemp() {
  char outputLine[20] = "";
  OrbitOledClear();
  OrbitOledSetCursor(0, 0);
  
  sprintf(outputLine, "Temp is: %g C", TempRead());
  OrbitOledPutString(outputLine);

  if (TempIsSet()) {
    OrbitOledSetCursor(0, 3);

    sprintf(outputLine, "Set temp is: %g C", GetDesiredTemp());
    OrbitOledPutString(outputLine);
  }
}

void SelectSchedule() {
}

void SelectTemp() {
}


