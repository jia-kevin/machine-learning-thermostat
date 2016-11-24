#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <string.h>
#include <stdbool.h>


bool IsMachineLearning = true;
bool Celcius = true;

static enum Pages {
  MenuDisplay      = 0,
  TempDisplay      = 1,
  ModeSelect       = 2,
  ModifySchedule   = 3,
} CurrentPage = TempDisplay;

static enum MenuOptions {
  ToTemp                 = 0,
  ToModeSelect           = 1,
  ToModifySchedule       = 2,
  MachineLearningToggle  = 3,
  CelciusFarenheitToggle = 4,
} CurrentMenuOption = ToTemp;


void DisplayMenu() {
  char outputLine[20] = "";
  switch (CurrentMenuOption) {
    case ToTemp:
      sprintf(outputLine, " Display Temperature ");
      break;
    case ToModeSelect:
      sprintf(outputLine, " Select Mode ");
      break;
    case ToModifySchedule:
      sprintf(outputLine, " Change Schedule ");
      break;
    case MachineLearningToggle:
      sprintf(outputLine, " Machine Learning: %s ", IsMachineLearning ? "ON" : "OFF");
      break;
    case CelciusFarenheitToggle:
      sprintf(outputLine, " %s ", Celcius ? "Celcius" : "Farenheit");
      break;
  }

  OrbitOledClear();
  OrbitOledSetCursor(0, 0);
  OrbitOledPutString(outputLine);
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

void DisplayTick() {
}


