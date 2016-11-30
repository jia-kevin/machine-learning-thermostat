#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <string.h>
#include <stdbool.h>
#include "globalconstants.h"
#include "datetime.h"

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
  SetTimeDisplay    = 5,          
} CurrentPage = MenuDisplay;

static enum MenuOptions {
  ToTemp                 = 0,
  ToModeSelect           = 1,
  ToTempSelect           = 2,
  ToModifySchedule       = 3,
  ToSetTimeDisplay       = 4,
  MachineLearningToggle  = 5,
  CelciusFarenheitToggle = 6,
} CurrentMenuOption = ToTemp;

static enum Modes {
  None                   = 0,
  Schedule               = 1,
  Vacation               = 2,
  PowerSaver             = 3,
  MachineLearning        = 4,
} CurrentMode = None;

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

    case ToSetTimeDisplay:
      sprintf(outputLine1, "Set Time");
      if (GetButtonEnter())
        CurrentPage = SetTimeDisplay;
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
  char outputLine1[ScreenWidth+1] = "";
  char outputLine2[ScreenWidth+1] = "";
  char outputLine3[ScreenWidth+1] = "";
  char outputLine4[ScreenWidth+1] = "";
  char timeString [ScreenWidth+1] = "";
  Serial.println("1");
  if (Celcius)
    sprintf(outputLine1, "Temp: %g C", TempRead());
  else
    sprintf(outputLine1, "Temp: %g F", CtoF(TempRead()));
    
  if (TempIsSet()) {
    if (Celcius)
      sprintf(outputLine2, "Set temp: %g C", GetDesiredTemp());
    else
      sprintf(outputLine2, "Set temp: %g F", CtoF(GetDesiredTemp()));
  }
  
  if (GetMode())
    sprintf(outputLine3, "%s", GetModeName(GetMode()));
  sprintf(outputLine4, "%s", LeftJustify(TimeToString(timeString)));
  
  OrbitOledClear();
  OrbitOledSetCursor(0, 0);
  OrbitOledPutString(outputLine1);
  OrbitOledSetCursor(0, 1);
  OrbitOledPutString(outputLine2);
  OrbitOledSetCursor(0, 2);
  OrbitOledPutString(outputLine3);
  OrbitOledSetCursor(0, 3);
  OrbitOledPutString(outputLine4);
  
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
  else {
    if (Celcius)
      sprintf(outputLine1, "Set temp: %g C", CurrSetTemp);
    else
      sprintf(outputLine1, "Set temp: %g F", CurrSetTemp);
  }
  CenterLine(outputLine1);

  if (GetPotentiometer() < 0.01) 
    sprintf(outputLine2, "Desired: NONE");
  else {
    if (Celcius)
      sprintf(outputLine2, "Desired: %g C", DesiredTempFromPot());
    else
      sprintf(outputLine2, "Desired: %g F", CtoF(DesiredTempFromPot()));
  }
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
      CurrentMode = None;
    }
  } 
  else if (GetButtonCancel())
    CurrentPage = MenuDisplay;
}

void DisplaySetTime() {
  const int numDays    = 7;
  const int numHours   = 12;
  const int numMin     = 60;
  const int numAMPM    = 2;
  
  static int currentSelect = 0; //0-3 option for whether selecting: Day, Hour, Min, AM/PM
  static int selectDay     = 0;
  static int selectHour    = 0;
  static int selectMin     = 0;
  static int selectAMPM    = 0;

  char outputLine1[ScreenWidth+1] = "";
  char outputLine3[ScreenWidth+1] = "";

  sprintf(outputLine1, "Select Time:");
  
  char printDay [4]; 
  char printHour[3];
  char printMin [3];
  char printAMPM[3];
  
  sprintf(printDay, "%s", GetDayofWeek(selectDay));
  sprintf(printHour, "%d", selectHour == 0 ? 12 : selectHour);
  sprintf(printMin, "%02d", selectMin);
  sprintf(printAMPM, "%s", (selectAMPM) ? "AM" : "PM");
  
  switch (currentSelect) {
    case 0:
      selectDay = (int)(GetPotentiometer()*numDays);
      sprintf(printDay, "%s", GetDayofWeek(selectDay));
      if (IsFlashing()) 
        sprintf(printDay, "   ");
      break;
    case 1:
      selectHour = (int)(GetPotentiometer()*numHours);
      sprintf(printHour, "%d", selectHour == 0 ? 12 : selectHour);
      if (IsFlashing()) {
        if (selectHour >=10 || selectHour == 0)
          sprintf(printHour, "  ");
        else
          sprintf(printHour, " ");
      }
      break;
    case 2:
      selectMin = (int)(GetPotentiometer()*numMin);
      sprintf(printMin, "%02d", selectMin);
      if (IsFlashing()) {
        sprintf(printMin, "  ");
      }
      break;
    case 3:
      selectAMPM = (int)(GetPotentiometer()*numAMPM);
      sprintf(printAMPM, "%s", (selectAMPM) ? "AM" : "PM");
      if (IsFlashing()) {
        sprintf(printAMPM, "  ");
      }
      break;            
  }
  sprintf(outputLine3, "%s %s:%s %s", printDay, printHour, printMin, printAMPM);

  if (GetButtonEnter()) {
    currentSelect = (currentSelect+1)%4;
    if (currentSelect == 0) {
      SetTime(selectDay, selectHour+12*selectAMPM, selectMin);
      CurrentPage = MenuDisplay;
    }
  }
  else if (GetButtonCancel()) {
    if (currentSelect == 0) {
      selectDay  = 0;
      selectHour = 0;
      selectMin  = 0;
      selectAMPM = 0;
      CurrentPage = MenuDisplay;
    }
    else
      currentSelect = (currentSelect-1+4)%4;
  }
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
    case SetTimeDisplay:
      DisplaySetTime();
      break;
  }
}

int GetMode() {
  return static_cast<int>(CurrentMode);
}

int GetNumModes() {
  return NumModes;
}

