#ifndef I_EXTRA_PEDAL_INTERFACE_H
#define I_EXTRA_PEDAL_INTERFACE_H

#include <Arduino.h>

class IExtraPedalInterface {
public:
    virtual uint32_t getExpressionValue() = 0;
    virtual bool getSwitchState(int i) = 0;
};

#endif