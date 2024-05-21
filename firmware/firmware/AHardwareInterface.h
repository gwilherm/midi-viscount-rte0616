#ifndef A_HARDWARE_INTERFACE_H
#define A_HARDWARE_INTERFACE_H

#include <Arduino.h>

class AHardwareInterface {
public:
    virtual int* getRawValues() = 0;
    virtual int8_t* getSegmentedValues() = 0;
};

#endif