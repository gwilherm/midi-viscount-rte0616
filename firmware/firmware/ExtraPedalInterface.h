#ifndef EXTRA_PEDAL_INTERFACE_H
#define EXTRA_PEDAL_INTERFACE_H

#include "IExtraPedalInterface.h"

#include <Arduino.h>

#define NB_SWITCH 5

class ExtraPedalInterface : public IExtraPedalInterface {
public:
    ExtraPedalInterface();
    virtual ~ExtraPedalInterface() = default;

    void setup();
    void loop();

    virtual uint32_t getExpressionValue() { return _expressionValue; };
    virtual bool getSwitchState(int i) { return (_switchState[i] == LOW); };

protected:
    const int _expressionPin = A10;
    const int _switchPin[NB_SWITCH] = {2, 3, 14, 15, 16};
    uint32_t _expressionValue;
    int _switchState[NB_SWITCH];
    int _lastSwitchState[NB_SWITCH];
    unsigned long _lastSwitchChange[NB_SWITCH];
};

#endif