#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <string.h>
#include <stdbool.h>

const static int ScreenWidth = 16;
const static int NumPages    = 4;
const static int MenuOptions = 5;

bool IsMachineLearning = true;
bool Celcius           = true;

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
  char outputLine1[ScreenWidth+1] = "";
  char outputLine2[ScreenWidth+1] = "";
  if (GetPotentiometer()<0 || GetPotentiometer() >= 1) Serial.println("ERROR");
  CurrentMenuOption = static_cast<enum MenuOptions>((int)(GetPotentiometer()*MenuOptions));
  switch (CurrentMenuOption) {
    case ToTemp:
      sprintf(outputLine1, "Display Temp");
      if (GetButtonEnter())
        CurrentPage = TempDisplay;
      break;
      
    case ToModeSelect:
      sprintf(outputLine1, "Select Mode");
      break;
      
    case ToModifySchedule:
      sprintf(outputLine1, "Change Schedule");
      break;
      
    case MachineLearningToggle:
      sprintf(outputLine1, "Machine Learning");
      sprintf(outputLine2, "%s", IsMachineLearning ? "ON" : "OFF");
      if (GetButtonEnter()) 
        IsMachineLearning = !IsMachineLearning;
      break;
      
    case CelciusFarenheitToggle:
      sprintf(outputLine1, "Degrees C/F");
      sprintf(outputLine2, "%s", Celcius ? "Celcius" : "Farenheit");
      if (GetButtonEnter()) 
        Celcius = !Celcius;
      break;
  }
  CenterLine(outputLine1);
  CenterLine(outputLine2);
  
  OrbitOledClear();
  OrbitOledSetCursor(0, 0);
  OrbitOledPutString(outputLine1);
  OrbitOledSetCursor(0, 1);
  OrbitOledPutString(outputLine2);
}

void DisplayTemp() {
  char outputLine[ScreenWidth+1] = "";
  OrbitOledClear();
  OrbitOledSetCursor(0, 0);
  
  sprintf(outputLine, "Temp: %g C", TempRead());
  OrbitOledPutString(outputLine);

  if (TempIsSet()) {
    OrbitOledSetCursor(0, 1);

    sprintf(outputLine, "Set temp: %g C", GetDesiredTemp());
    OrbitOledPutString(outputLine);
  }
  if (GetButtonCancel())
    CurrentPage = MenuDisplay;
}

void SelectSchedule() {
}

void SelectTemp() {
}

void DisplayTick() {
  switch(CurrentPage) {
    case MenuDisplay:
      DisplayMenu();
      break;
    case TempDisplay:
      DisplayTemp();
      break;
  }
}

char* CenterLine(char line[]) {
  char result[ScreenWidth+1] = "";
  char spaces[ScreenWidth+1] = "";
  int i = 0;
  
  for (i=0; i<(ScreenWidth-strlen(line))/2; i++) 
    spaces[i] = ' ';
  spaces[i] = '\0';

  strcat(result, spaces);
  strcat(result, line);
  strcat(result, spaces);

  strcpy(line, result);

  return line;
}

