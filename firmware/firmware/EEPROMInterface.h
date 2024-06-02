#ifndef EEPROM_INTERFACE_H
#define EEPROM_INTERFACE_H

#include "IEEPROMInterface.h"

class EEPROMInterface: public IEEPROMInterface
{
public:
    EEPROMInterface();
    virtual ~EEPROMInterface() = default;

    virtual uint8_t read( int idx );
    virtual void update( int idx, uint8_t val );
};

#endif