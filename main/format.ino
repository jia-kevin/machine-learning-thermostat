#include <math.h>
#include <stdbool.h>
#include <string.h>

#define EPS 1e-9

static const int  ScreenWidth      = 16;
static const char DaysofWeek[7][4] = {
  "Mon",
  "Tue", 
  "Wed",
  "Thu",
  "Fri",
  "Sat",
  "Sun"
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
