#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <string.h>
#include <stdbool.h>

static const int NumPages       = 5;
static const int NumMenuOptions = 6;
static const int NumModes       = 5;

static float CurrSetTemp;
static float TempSetTemp;

bool IsMachineLearning = true;
bool Celcius           = true;

static enum Pages {
  MenuDisplay       = 0,
  TempDisplay       = 1,
  ModeSelect        = 2,
  TempSelectDisplay = 3,
  ModifySchedule    = 4,
} CurrentPage = TempDisplay;

static enum MenuOptions {
  ToTemp                 = 0,
  ToModeSelect           = 1,
  ToTempSelect           = 2,
  ToModifySchedule       = 3,
  MachineLearningToggle  = 4,
  CelciusFarenheitToggle = 5,
} CurrentMenuOption = ToTemp;

static enum Modes {
  None                   = 0,
  Schedule               = 1,
  Vacation               = 2,
  PowerSaver             = 3,
  MachineLearning        = 4,
} CurrentMode = None;

static const char ModeNames[NumModes][ScreenWidth+1]  = {
  "None",
  "Schedule",
  "Vacation",
  "PowerSaver",
  "Mach. L"
};

void UiInit() {
  CurrSetTemp = GetNoSetTemp();
  TempSetTemp = GetNoSetTemp();
}

void DisplayMenu() {
  char outputLine1[ScreenWidth+1] = "";
  char outputLine2[ScreenWidth+1] = "";
  
  CurrentMenuOption = static_cast<enum MenuOptions>((int)(GetPotentiometer()*NumMenuOptions));
  switch (CurrentMenuOption) {
    case ToTemp:
      sprintf(outputLine1, "Display Temp");
      if (GetButtonEnter())
        CurrentPage = TempDisplay;
      break;
      
    case ToModeSelect:
      sprintf(outputLine1, "Select Mode");
      if (GetButtonEnter()) 
        CurrentPage = ModeSelect;
      break;

    case ToTempSelect:
      sprintf(outputLine1, "Select Temp");
      if (GetButtonEnter()) 
        CurrentPage = TempSelectDisplay;
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

void SelectMode() {
  char outputLine1[ScreenWidth+1] = "";
  char outputLine2[ScreenWidth+1] = "";

  sprintf(outputLine1, "Curr: %s", ModeNames[CurrentMode]);
  sprintf(outputLine2, "%s", ModeNames[(int)(GetPotentiometer()*NumModes)]);

  CenterLine(outputLine2);
  
  OrbitOledClear();
  OrbitOledSetCursor(0, 0);
  OrbitOledPutString(outputLine1);
  OrbitOledSetCursor(0, 1);
  OrbitOledPutString(outputLine2);

  if (GetButtonEnter()) 
    CurrentMode = static_cast<enum Modes>((int)(GetPotentiometer()*NumModes));
  if (GetButtonCancel())
    CurrentPage = MenuDisplay;
}

void SelectSchedule() {
}

void SelectTemp() {
  char outputLine1[ScreenWidth+1] = "";
  char outputLine2[ScreenWidth+1] = "";

  if (CurrSetTemp == GetNoSetTemp())
    sprintf(outputLine1, "Temp not set!");
  else
    sprintf(outputLine1, "Set temp: %g", CurrSetTemp);
  CenterLine(outputLine1);

  if (GetPotentiometer() < 0.01) 
    sprintf(outputLine2, "Desired: NONE");
  else
    sprintf(outputLine2, "Desired: %g", DesiredTempFromPot());
  CenterLine(outputLine2);
  
  OrbitOledClear();
  OrbitOledSetCursor(0, 0);
  OrbitOledPutString(outputLine1);
  OrbitOledSetCursor(0, 2);
  OrbitOledPutString(outputLine2);

  if (GetButtonEnter()){
    if (GetPotentiometer() < 0.01)
      CurrSetTemp = GetNoSetTemp();
    else
      CurrSetTemp = DesiredTempFromPot();

    SetDesiredTemp(CurrSetTemp);
    
    if (CurrentMode != None && CurrentMode != MachineLearning) {
      if (IsMachineLearning) CurrentMode = MachineLearning;
      else CurrentMode = None;
    }
  } 
  else if (GetButtonCancel())
    CurrentPage = MenuDisplay;
}

void DisplayTick() {
  switch(CurrentPage) {
    case MenuDisplay:
      DisplayMenu();
      break;
    case TempDisplay:
      DisplayTemp();
      break;
    case ModeSelect:
      SelectMode();
      break;
    case TempSelectDisplay:
      SelectTemp();
      break;
  }
}
