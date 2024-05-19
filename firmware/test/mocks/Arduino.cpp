#include "Arduino.h"

#include "ArduinoMock.h"

void pinMode(uint8_t pin, uint8_t mode)
{
    ArduinoMock::getInstance().pinMode(pin, mode);
}

int analogRead(uint8_t pin)
{
    return ArduinoMock::getInstance().analogRead(pin);
}

unsigned long millis(void)
{
    return ArduinoMock::getInstance().millis();
}
