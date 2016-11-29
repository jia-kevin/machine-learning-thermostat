//7 days, 24 hours per day, 4 intervals per hour (15 min)
//672 divisions per week, weekly schedules
const static int Eeprom                 = EEPROMADDR;
const static int ScheduleArrayElements  = 672;
const static int ScheduleArraySize      = ScheduleArrayElements*2; //need 1 byte for integer, 1 byte for .5 or .0

void EepromInit() { 
}

void EepromRead(float *readTo, int scheduleNum) {
  short memAddress = scheduleNum * ScheduleArraySize;
  
  for (int i=0; i<ScheduleArrayElements; i++) {
    int mostSignificantBit    = (memAddress+2*i) >> 8;
    int leastSignificantBit   = (memAddress+2*i) & 0xFF;
    size_t const DataLength   = 1;
    uint32_t data[DataLength] = { 0 };
    
    WireWriteRegister(Eeprom, mostSignificantBit,  leastSignificantBit);
    WireRequestArray(Eeprom, data, DataLength);
    readTo[i] = data[0];
    if (readTo[i] > 128)
      readTo[i] -= 256;

    mostSignificantBit        = (memAddress+2*i+1) >> 8;
    leastSignificantBit       = (memAddress+2*i+1) & 0xFF;
    
    WireWriteRegister(Eeprom, mostSignificantBit,  leastSignificantBit);
    WireRequestArray(Eeprom, data, DataLength);
    if (data[0] == 128)
      readTo[i] += 0.5;
  }
}
