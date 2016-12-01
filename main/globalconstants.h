#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

const int MinSetTemp     = 15;
const int MaxSetTemp     = 35;
const int NoSetTemp      = MinSetTemp - 1;

const int ScreenWidth    = 16;

const int NumPages       = 6;
const int NumMenuOptions = 7;
const int NumModes       = 5;

const int DaysInWeek             = 7;
const int HoursInDay             = 24;
const int IntervalsInHour        = 4;
const int MinutesInInterval      = 15;
const int ScheduleArrayElements  = DaysInWeek * HoursInDay * IntervalsInHour;
const int ScheduleArraySize      = 2 * ScheduleArrayElements;

#endif
