//2D arrays, 7 days, 24*4 = 96 divisions per day

#include <EEPROM.h>
#define mskHB       0x00007F00    // bit 15 is a don't care for memory addressing
#define mskLB       0x000000FF

const static int Eeprom = EEPROMADDR;
const static int ScheduleArrayElements = 672;
const static int ScheduleArraySize = ScheduleArrayElements*2; //need 1 byte for integer, 1 byte for .5 or .0

void EepromInit() {
  
}

void EepromRead(char *readTo, int scheduleNum) {
  short memAddress = scheduleNum * ScheduleArraySize;
  
    bMemAddrHB = (bAddr & mskHB) >> 8;
    bMemAddrLB = (bAddr & mskLB);
}
