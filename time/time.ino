static unsigned long day;
static unsigned long hour;
static unsigned long min;
static unsigned long time;
static unsigned long change;


int testCounter;
/*void setup() {
  Serial.begin(9600);
  SetTime(3, 7, 51);
  Serial.println(day);
  Serial.println(hour);
  Serial.println(min);
  Serial.println(time);

  change = 0;
  
}*/

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

/*void loop() {
  // put your main code here, to run repeatedly: 
  testCounter++;
  //Serial.println(testCounter);
  if(testCounter==10000000)
  {
    GetTime();
  Serial.println(day);
  Serial.println(hour);
  Serial.println(min);
  Serial.println(time);
  Serial.println(change);

  }
  if(testCounter==12000000) {
    unsigned long pass = millis();
    Serial.println(pass);
    GetTime();
  Serial.println(day);
  Serial.println(hour);
  Serial.println(min);
  Serial.println(time);
  Serial.println(change);
  }
}*/

