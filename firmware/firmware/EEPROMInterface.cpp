#include "EEPROMInterface.h"

#include <EEPROM.h>

EEPROMInterface::EEPROMInterface()
{}

uint8_t EEPROMInterface::read(int idx)
{
    return EEPROM.read(idx);
}

void EEPROMInterface::update(int idx, uint8_t val)
{
    EEPROM.update(idx, val);
}