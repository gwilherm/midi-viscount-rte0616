#ifndef ARDUINO_H
#define ARDUINO_H

#include <cstdint>

#define INPUT_PULLUP 1

enum AnalogPin {
    A0,
    A1,
    A2,
    A3,
    A6,
    A7,
    A8,
    A9
};

void pinMode(uint8_t, uint8_t);
int analogRead(uint8_t);
unsigned long millis(void);

#endif