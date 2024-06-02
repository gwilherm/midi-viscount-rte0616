#ifndef I_EEPROM_INTERFACE_H
#define I_EEPROM_INTERFACE_H

#include <Arduino.h>

class IEEPROMInterface
{
public:
    virtual uint8_t read( int idx ) = 0;
    virtual void update( int idx, uint8_t val ) = 0;
};

#endif