#include "ExtraPedalInterface.h"

ExtraPedalInterface::ExtraPedalInterface():
    _expressionValue(1023),
    _switchState{HIGH, HIGH, HIGH, HIGH, HIGH},
    _lastSwitchState{HIGH, HIGH, HIGH, HIGH, HIGH},
    _lastSwitchChange{0, 0, 0, 0, 0}
{}

void ExtraPedalInterface::setup()
{
    // Expression pedal (last analog pin)
    pinMode(_expressionPin, INPUT_PULLUP);

    // Foot switches
    for (int i = 0; i < NB_SWITCH; i++)
        pinMode(_switchPin[i], INPUT_PULLUP);
}

void ExtraPedalInterface::loop()
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