static unsigned long day;
static unsigned long hour;
static unsigned long min;
static unsigned long time;
static unsigned long change;

void TimeInit() {
  change = 0;
}

void SetTime(unsigned long a, unsigned long b, unsigned long c) {
  day = a;
  hour = b;
  min = c;
  time = millis();
}

void GetTime() {
  change = millis() - time;
  unsigned long addMin = change / 60000;
  min += addMin;
  unsigned long addHour = min / 60;
  hour += addHour;
  min = min % 60;
  unsigned long addDay = hour / 24;
  day += addDay;
  hour = hour % 24;
  day = day % 7;
  SetTime(day, hour, min);
}

