#ifndef AM32_M030_EEPROM_H
#define AM32_M030_EEPROM_H

#include "main.h"

bool save_flash_nolib(const uint8_t *data, uint32_t length, uint32_t address);
void read_flash_bin(uint8_t *data, uint32_t address, int length);

#endif
