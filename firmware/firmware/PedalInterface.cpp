#include "PedalInterface.h"

PedalInterface::PedalInterface():
    _expressionValue(1023),
    _lastSwitchState{HIGH},
    _lastSwitchChange{0}
{}

void PedalInterface::setup()
{
    // Expression pedal (last analog pin)
    pinMode(_expressionPin, INPUT_PULLUP);

    // Foot switches
    for (int i = 0; i < NB_SWITCH; i++)
        pinMode(_switchPin[i], INPUT_PULLUP);
}

void PedalInterface::loop()
{
    _expressionValue = analogRead(_expressionPin);

    unsigned long time = millis();
    for (int i = 0; i < NB_SWITCH; i++)
    {
        int state = digitalRead(_switchPin[i]);

        if (state != _lastSwitchState[i])
            _lastSwitchChange[i] = time;

        if ((time - _lastSwitchChange[i]) > 50)
            _switchState[i] = state;

        _lastSwitchState[i] = state;
    }
}