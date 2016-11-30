#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "datetime.h"
#include "globalconstants.h"

#define EPS 1e-9

static char DaysofWeek[7][4] = {
  "Mon",
  "Tue", 
  "Wed",
  "Thu",
  "Fri",
  "Sat",
  "Sun"
};

static char ModeNames[NumModes][ScreenWidth+1]  = {
  "None",
  "Schedule",
  "Vacation",
  "PowerSaver",
  "Mach. L"
};

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

char* LeftJustify(char line[]) {
  char result[ScreenWidth+1] = "";
  char spaces[ScreenWidth+1] = "";
  int i = 0;
  
  for (i=0; i<(ScreenWidth-strlen(line)); i++) 
    spaces[i] = ' ';
  spaces[i] = '\0';

  strcat(result, spaces);
  strcat(result, line);

  strcpy(line, result);

  return line;
}

float DesiredTempFromPot() {
  return (((int)(GetPotentiometer() * (GetMaxTemp() - GetMinTemp()) * 2)) / 2.0 + GetMinTemp());
}

float FtoC (float F) {
  if (fabs(F - GetNoSetTemp()) < EPS)
    return GetNoSetTemp();
  return (5 * (F - 32) / 9);
}

float CtoF(float C) {
  if (fabs(C - GetNoSetTemp()) < EPS)
    return GetNoSetTemp();
  return (9 * C / 5 + 32);
}

const char* GetModeName(int i) {
  return ModeNames[i];
}

char* TimeToString(char* input) {
  struct DateTime currTime = GetTime();
  char result[ScreenWidth+1] = "";

  sprintf(result, "%s %d:%02d %s", DaysofWeek[currTime.day], 
                                 (currTime.hour%12 == 0) ? 12 : currTime.hour%12, 
                                 currTime.minute,
                                 (currTime.hour>=12) ? "PM" : "AM");

  strcpy(input, result);
  return input;
}

char* GetDayofWeek(int i) {
  return DaysofWeek[i];
}

bool IsFlashing() {
  return (millis()/500)%2;
}

