#ifndef I_PEDAL_INTERFACE_H
#define I_PEDAL_INTERFACE_H

#include <Arduino.h>

class IPedalInterface {
public:
    virtual uint32_t getExpressionValue() = 0;
    virtual bool getSwitchState(int i) = 0;
};

#endif