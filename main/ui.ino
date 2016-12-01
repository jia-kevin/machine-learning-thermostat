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

float TenativeSchedule[ScheduleArrayElements];

bool IsMachineLearning = true;
bool Celcius           = true;
bool IsEditingSchedule = false;

static enum Pages {
  MenuDisplay        = 0,
  TempDisplay        = 1,
  ModeSelect         = 2,
  TempSelectDisplay  = 3,
  ModifyScheduleMenu = 4,
  SetTimeDisplay     = 5,          
} CurrentPage = TempDisplay;

static enum MenuOptions {
  ToTemp                 = 0,
  ToModeSelect           = 1,
  ToTempSelect           = 2,
  ToModifySchedule       = 3,
  ToSetTimeDisplay       = 4,
  MachineLearningToggle  = 5,
  CelciusFarenheitToggle = 6,
} CurrentMenuOption = ToTemp;

static enum ScheduleMenuOptions {
  EditSchedule           = 0,
  SaveSchedule           = 1,
  ClearSchedule          = 2,
} CurrentScheduleMenuOption = EditSchedule;

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
  char outputLine3[ScreenWidth+1] = "";
  char outputLine4[ScreenWidth+1] = "";
  
  sprintf(outputLine1, "MENU");
  
  CurrentMenuOption = static_cast<enum MenuOptions>((int)(GetPotentiometer()*NumMenuOptions));
  switch (CurrentMenuOption) {
    case ToTemp:
      sprintf(outputLine3, "Display Temp");
      if (GetButtonEnter())
        CurrentPage = TempDisplay;
      break;
      
    case ToModeSelect:
      sprintf(outputLine3, "Select Mode");
      if (GetButtonEnter()) 
        CurrentPage = ModeSelect;
      break;

    case ToTempSelect:
      sprintf(outputLine3, "Select Temp");
      if (GetButtonEnter()) 
        CurrentPage = TempSelectDisplay;
      break;
      
    case ToModifySchedule:
      sprintf(outputLine3, "Change Schedule");
      if (GetButtonEnter()) 
        CurrentPage = ModifyScheduleMenu;
      break;

    case ToSetTimeDisplay:
      sprintf(outputLine3, "Set Time");
      if (GetButtonEnter())
        CurrentPage = SetTimeDisplay;
      break;
    
    case MachineLearningToggle:
      sprintf(outputLine3, "Machine Learning");
      sprintf(outputLine4, "%s", IsMachineLearning ? "ON" : "OFF");
      if (GetButtonEnter()) 
        IsMachineLearning = !IsMachineLearning;
      break;
      
    case CelciusFarenheitToggle:
      sprintf(outputLine3, "Degrees C/F");
      sprintf(outputLine4, "%s", Celcius ? "Celcius" : "Farenheit");
      if (GetButtonEnter()) 
        Celcius = !Celcius;
      break;
  }
  CenterLine(outputLine1);
  CenterLine(outputLine3);
  CenterLine(outputLine4);
  
  PrintScreen(outputLine1, outputLine2, outputLine3, outputLine4);
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
  
  PrintScreen(outputLine1, outputLine2, outputLine3, outputLine4);
  
  if (GetButtonCancel())
    CurrentPage = MenuDisplay;
}

void SelectMode() {
  char outputLine1[ScreenWidth+1] = "";
  char outputLine2[ScreenWidth+1] = "";
  char outputLine3[ScreenWidth+1] = "";
  char outputLine4[ScreenWidth+1] = "";

  sprintf(outputLine1, "Curr: %s", ModeNames[CurrentMode]);
  sprintf(outputLine2, "%s", ModeNames[(int)(GetPotentiometer()*NumModes)]);

  CenterLine(outputLine2);
  
  PrintScreen(outputLine1, outputLine2, outputLine3, outputLine4);

  if (GetButtonEnter()) 
    CurrentMode = static_cast<enum Modes>((int)(GetPotentiometer()*NumModes));
  if (GetButtonCancel())
    CurrentPage = MenuDisplay;
}

void SelectScheduleMenu() {
  const int numOptions = 3;

  char outputLine1[ScreenWidth+1] = "";
  char outputLine2[ScreenWidth+1] = "";
  char outputLine3[ScreenWidth+1] = "";
  char outputLine4[ScreenWidth+1] = "";

  sprintf(outputLine1, "Schedule Menu:");
  CenterLine(outputLine1);
  if (IsEditingSchedule) {
    DisplayEditSchedule();
  }
  else {
    CurrentScheduleMenuOption = static_cast<enum ScheduleMenuOptions>((int)(GetPotentiometer()*numOptions));
    switch (CurrentScheduleMenuOption) {
      case EditSchedule:
        sprintf(outputLine3, "Edit Schedule");
        if (GetButtonEnter()) {
          IsEditingSchedule = true;
        }
        break;
        
      case SaveSchedule:
        sprintf(outputLine3, "Save Schedule");
        if (GetButtonEnter()) {
          SaveNewSchedule(TenativeSchedule, Schedule);
        }
        break;
        
      case ClearSchedule:
        sprintf(outputLine3, "Clear Schedule");
        if (GetButtonEnter()) {
          for (int i=0; i<ScheduleArrayElements; i++) {
            TenativeSchedule[i] = GetNoSetTemp();
          }
        }
        break;
    }
    CenterLine(outputLine3);
    
    PrintScreen(outputLine1, outputLine2, outputLine3, outputLine4);
    
    if (GetButtonCancel())
      CurrentPage = MenuDisplay;
  }
}

void DisplayEditSchedule() {
  const int numDays    = 10; //(7 - Weekday, 8 - Weekend)
  const int numHours   = 12;
  const int numMin     = IntervalsInHour;
  const int numAMPM    = 2;
  
  static int currentSelect    = 0; //0-7 option for whether selecting: Day, Hour, Min, AM/PM
  static int selectDay        = {0};
  static int selectHour[2]    = {0};
  static int selectMin [2]    = {0};
  static int selectAMPM[2]    = {0};
  static float selectTemp     = GetNoSetTemp();

  char outputLine1[ScreenWidth+1] = "";
  char outputLine2[ScreenWidth+1] = "";
  char outputLine3[ScreenWidth+1] = "";
  char outputLine4[ScreenWidth+1] = "";
 
  char printDay [6]; 
  char printHour[2][3];
  char printMin [2][3];
  char printAMPM[2][3];
  char printTemp[5];
  if (selectTemp == GetNoSetTemp())
    sprintf(printTemp, "NONE");
  else {
    if (Celcius)
      sprintf(printTemp, "%g C", selectTemp);
    else
      sprintf(printTemp, "%g F", CtoF(selectTemp));
  }
  
  sprintf(printDay, "%s", GetDayofWeek(selectDay));
  
  sprintf(printHour[0], "%2d", selectHour[0] == 0 ? 12 : selectHour[0]);
  sprintf(printMin[0], "%02d", selectMin[0]*MinutesInInterval);
  sprintf(printAMPM[0], "%s", selectAMPM[0] == 0 ? "AM" : "PM");
  
  sprintf(printHour[1], "%2d", selectHour[1] == 0 ? 12 : selectHour[1]);
  sprintf(printMin[1], "%02d", selectMin[1]*MinutesInInterval);
  sprintf(printAMPM[1], "%s", selectAMPM[1] == 0 ? "AM" : "PM");

  switch (currentSelect) {
    case 0:
      selectDay = (int)(GetPotentiometer()*numDays);
      if (selectDay <= 6) {
        sprintf(printDay, "%s", GetDayofWeek(selectDay));
        if (IsFlashing()) 
          sprintf(printDay, "   ");
      }
      if (selectDay == 7) {
        sprintf(printDay, "%s", "WKDAY");
        if (IsFlashing()) 
          sprintf(printDay, "     ");
      }
      if (selectDay == 8) {
        sprintf(printDay, "%s", "WKEND");
        if (IsFlashing()) 
          sprintf(printDay, "     ");
      }
      if (selectDay == 9) {
        sprintf(printDay, "%s", "ALL");
        if (IsFlashing()) 
          sprintf(printDay, "   ");
      }
      break;
      
    case 1:
      selectHour[0] = (int)(GetPotentiometer()*numHours);
      sprintf(printHour[0], "%2d", selectHour[0] == 0 ? 12 : selectHour[0]);
      if (IsFlashing()) {
        sprintf(printHour[0], "  ");
      }
      break;
      
    case 2:
      selectMin[0] = (int)(GetPotentiometer()*numMin);
      sprintf(printMin[0], "%02d", selectMin[0]*MinutesInInterval);
      if (IsFlashing()) {
        sprintf(printMin[0], "  ");
      }
      break;
      
    case 3:
      selectAMPM[0] = (int)(GetPotentiometer()*numAMPM);
      sprintf(printAMPM[0], "%s", selectAMPM[0] == 0 ? "AM" : "PM");
      if (IsFlashing()) {
        sprintf(printAMPM[0], "  ");
      }
      break;
      
    case 4:
      selectHour[1] = (int)(GetPotentiometer()*numHours);
      sprintf(printHour[1], "%2d", selectHour[1] == 0 ? 12 : selectHour[1]);
      if (IsFlashing()) {
        sprintf(printHour[1], "  ");
      }
      break;
      
    case 5:
      selectMin[1] = (int)(GetPotentiometer()*numMin);
      sprintf(printMin[1], "%02d", selectMin[1]*MinutesInInterval);
      if (IsFlashing()) {
        sprintf(printMin[1], "  ");
      }
      break;
      
    case 6:
      selectAMPM[1] = (int)(GetPotentiometer()*numAMPM);
      sprintf(printAMPM[1], "%s", selectAMPM[1] == 0 ? "AM" : "PM");
      if (IsFlashing()) {
        sprintf(printAMPM[1], "  ");
      }
      break;
      
    case 7:
      selectTemp = DesiredTempFromPot();
      if (selectTemp == GetNoSetTemp()){
        sprintf(printTemp, "NONE");
      }
      else {
        if (Celcius)
          sprintf(printTemp, "%g C", selectTemp);
        else
          sprintf(printTemp, "%g F", CtoF(selectTemp));
      }
      if (IsFlashing()) {
        char spaces[strlen(printTemp)+1];
        int i = 0;
        for (i=0; i<strlen(printTemp); i++) 
          spaces[i] = ' ';
        spaces[i] = '\0';
        sprintf(printTemp, "%s", spaces); 
      }
      break;
  }
  sprintf(outputLine1, "Day:   %s", printDay);
  sprintf(outputLine2, "Start: %s:%s %s", printHour[0], printMin[0], printAMPM[0]);
  sprintf(outputLine3, "End:   %s:%s %s", printHour[1], printMin[1], printAMPM[1]);
  sprintf(outputLine4, "Temp:  %s", printTemp);
  
  PrintScreen(outputLine1, outputLine2, outputLine3, outputLine4);

  if (GetButtonEnter()) {
    currentSelect = (currentSelect+1)%8;
    if (currentSelect == 0) {
      if (TimeCompareFirst(selectHour[0] + 12*selectAMPM[0], selectMin[0], 
                           selectHour[1] + 12*selectAMPM[1], selectMin[1])) {
        sprintf(outputLine2, "Confirmed!");
        strcpy(outputLine1, "");
        strcpy(outputLine3, "");
        strcpy(outputLine4, "");
        PrintScreen(outputLine1, outputLine2, outputLine3, outputLine4);
        delay(2000);

        if (selectDay <= 6) {
          for (int i=0; i<(selectHour[1] + 12*selectAMPM[1])*IntervalsInHour+selectMin[1] - 
                          ((selectHour[0] + 12*selectAMPM[0])*IntervalsInHour+selectMin[0]); i++)
            TenativeSchedule[selectDay*HoursInDay*IntervalsInHour 
                          + (selectHour[0] + 12*selectAMPM[0])*IntervalsInHour+selectMin[0]] = selectTemp;
        }
        else if (selectDay == 7){
          for (int j=0; j<5; j++)
          for (int i=0; i<(selectHour[1] + 12*selectAMPM[1])*IntervalsInHour+selectMin[1] - 
                          ((selectHour[0] + 12*selectAMPM[0])*IntervalsInHour+selectMin[0]); i++)
            TenativeSchedule[j*HoursInDay*IntervalsInHour 
                          + (selectHour[0] + 12*selectAMPM[0])*IntervalsInHour+selectMin[0]] = selectTemp;
        }
        else if (selectDay == 8){
          for (int j=5; j<7; j++)
          for (int i=0; i<(selectHour[1] + 12*selectAMPM[1])*IntervalsInHour+selectMin[1] - 
                          ((selectHour[0] + 12*selectAMPM[0])*IntervalsInHour+selectMin[0]); i++)
            TenativeSchedule[j*HoursInDay*IntervalsInHour 
                          + (selectHour[0] + 12*selectAMPM[0])*IntervalsInHour+selectMin[0]] = selectTemp;
        }
        else if (selectDay == 9){
          for (int j=0; j<7; j++)
          for (int i=0; i<(selectHour[1] + 12*selectAMPM[1])*IntervalsInHour+selectMin[1] - 
                          ((selectHour[0] + 12*selectAMPM[0])*IntervalsInHour+selectMin[0]); i++)
            TenativeSchedule[j*HoursInDay*IntervalsInHour 
                          + (selectHour[0] + 12*selectAMPM[0])*IntervalsInHour+selectMin[0]] = selectTemp;
        }
        IsEditingSchedule = false;
      }
      else {
        sprintf(outputLine2, "Invalid Times!");
        CenterLine(outputLine2);
        strcpy(outputLine1, "");
        strcpy(outputLine3, "");
        strcpy(outputLine4, "");
        PrintScreen(outputLine1, outputLine2, outputLine3, outputLine4);
        delay(2000);
        currentSelect = 1;
      }
    }
  }
  else if (GetButtonCancel()) {
    if (currentSelect == 0) {
      selectDay     = 0;
      selectHour[0] = 0;
      selectHour[1] = 0;
      selectMin [0] = 0;
      selectMin [1] = 0;
      selectAMPM[0] = 0;
      selectAMPM[1] = 0;

      IsEditingSchedule = false;
    }
    else
      currentSelect = (currentSelect-1+8)%8;
  }
}

void SelectTemp() {
  char outputLine1[ScreenWidth+1] = "";
  char outputLine2[ScreenWidth+1] = "";
  char outputLine3[ScreenWidth+1] = "";
  char outputLine4[ScreenWidth+1] = "";


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
    sprintf(outputLine3, "Desired: NONE");
  else {
    if (Celcius)
      sprintf(outputLine3, "Desired: %g C", DesiredTempFromPot());
    else
      sprintf(outputLine3, "Desired: %g F", CtoF(DesiredTempFromPot()));
  }
  CenterLine(outputLine3);
  
  PrintScreen(outputLine1, outputLine2, outputLine3, outputLine4);

  if (GetButtonEnter()){
    TempChangeToTrue();
    
    if (GetPotentiometer() < 0.01)
      CurrSetTemp = GetNoSetTemp();
    else
      CurrSetTemp = DesiredTempFromPot();

    SetDesiredTemp(CurrSetTemp);
    
    if (CurrentMode != None) {
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
  char outputLine2[ScreenWidth+1] = "";
  char outputLine3[ScreenWidth+1] = "";
  char outputLine4[ScreenWidth+1] = "";

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
      sprintf(printAMPM, "%s", (selectAMPM == 0) ? "AM" : "PM");
      if (IsFlashing()) {
        sprintf(printAMPM, "  ");
      }
      break;            
  }
  sprintf(outputLine3, "%s %s:%s %s", printDay, printHour, printMin, printAMPM);
  CenterLine(outputLine3);
  
  PrintScreen(outputLine1, outputLine2, outputLine3, outputLine4);
  
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
  if (!GetSwitchControlLock()) {
    DisplayTemp();
    return;
  }
  switch(CurrentPage) {
    case MenuDisplay:
      DisplayMenu();
      break;
    case TempDisplay:
      DisplayTemp();
      break;
    case ModifyScheduleMenu:
      SelectScheduleMenu();
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

bool GetIsMachineLearning() {
  return IsMachineLearning;
}

