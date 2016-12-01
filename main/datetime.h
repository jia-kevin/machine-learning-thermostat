#ifndef DATETIME_H
#define DATETIME_H

struct DateTime {
  int           day;  //0-monday, 6-sunday
  int           hour; //24 hour;
  int           minute;
  uint64_t      millisecondTime;
};

#endif
