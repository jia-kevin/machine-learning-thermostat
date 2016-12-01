#include "datetime.h"

struct DateTime InitialTime;

/*
 * uint64_t millis64()
 * 
 * Returns a 64 bit int instead of 32 bit int of number of
 * milliseconds passed.
 * Needed to avoid the millis() rollover at approx. 47 days.
 */
uint64_t millis64() {
    static uint32_t low32;
    static uint32_t high32;
    
    uint32_t new_low32 = millis();
    if (new_low32 < low32) high32++;
    low32 = new_low32;
    
    return ((uint64_t) high32 << 32 | low32);
}

void TimeInit() {
  InitialTime = {0, 0, 0, millis64()};
}

void SetTime(int newDay, int newHour, int newMinute) {
  InitialTime.day             = newDay;
  InitialTime.hour            = newHour;
  InitialTime.minute          = newMinute;
  InitialTime.millisecondTime = millis64();
}

struct DateTime GetTime() {
  struct DateTime current;
  current.day      = InitialTime.day;
  current.hour     = InitialTime.hour;
  current.minute   = InitialTime.minute;
  uint64_t change  = millis64() - InitialTime.millisecondTime;

  uint64_t addMin  = change  / 60000;
  uint64_t addHour = addMin  / 60;
  uint64_t addDay  = addHour / 24;
  uint64_t carry   = 0;

  current.minute += addMin % 60; 
  if (current.minute >= 60) {
    carry = current.minute/60;
    current.minute %= 60;
  }
  
  current.hour += addHour%24 + carry;
  if (current.hour >= 24) {
    carry = current.hour/24;
    current.hour %= 24;
  }
  
  current.day += addDay%7 + carry;
  if (current.day >= 7) {
    current.day %= 7;
  }

  return current;
}

/**
 * TimeCompareFirst(int hour1, int min1, int hour2, int min2)
 * 
 * compares two times (time of day: hour&min) and 
 * returns true if the first time is first
 * returns false if the second time is first or at the same time
 */
bool TimeCompareFirst(int hour1, int min1, int hour2, int min2) {
  int minutes1 = 60*hour1 + min1;
  int minutes2 = 60*hour2 + min2;

  return (minutes1 < minutes2);
}

