struct DateTime {
  int           day;  //0-monday, 6-sunday
  int           hour; //24 hour;
  int           minute;
  unsigned long millisecondTime;
};

struct DateTime InitialTime;

void TimeInit() {
  InitialTime = {0, 0, 0, 0};
}

void SetTime(int newDay, int newHour, int newMinute) {
  InitialTime.day             = newDay;
  InitialTime.hour            = newHour;
  InitialTime.minute          = newMinute;
  InitialTime.millisecondTime = millis();
}

struct DateTime GetTime() {
  struct DateTime current;
  current.day           = InitialTime.day;
  current.hour          = InitialTime.hour;
  current.minute        = InitialTime.minute;
  unsigned long change  = millis() - InitialTime.millisecondTime;

  unsigned long addMin  = (change / 60000);
  unsigned long addHour = addMin / 60;
  unsigned long addDay  = addHour / 24;
  unsigned long carry   = 0;

  current.minute += addMin % 60; 
  if (current.minute >= 60) {
    carry = current.minute/60;
    current.minute %= 60;
  }
  
  current.hour += addHour%24 + carry;
  if (current.hour >= 24) {
    carry = current.hour/24;
    current.hour %= 24
  }
  
  current.day += addDay%7 + carry;
  if (current.day >= 7) {
    current.day %= 7;
  }

  return current;
}

