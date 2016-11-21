#ifndef TWOWIRE_H
#define TWOWIRE_H
#include <Wire.h>

static TwoWire orbitWire(0);

void WireInit();

void WireWriteByte(int address, uint8_t value);

void WireWriteRegister(int address, uint8_t reg, uint8_t value);

/**
 * WireRequestArray(int address, uint32_t* buffer, uint8_t amount)
 * 
 * address - I2C Address of Chip
 * buffer - Buffer to store data
 * amount - Bytes of information to store!
 */
void WireRequestArray(int address, uint32_t* buffer, uint8_t amount);

#endif
