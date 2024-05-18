#ifndef HARDWARE_INTERFACE_H
#define HARDWARE_INTERFACE_H

#include "CalibrationConfig.h"
#include "pdlbrdkeys.h"

class HardwareInterface {
public:
    HardwareInterface(CalibrationConfig& calibrationConfig);
    virtual ~HardwareInterface() = default;

    void setup();
    void loop();

    int* getRawValues() { return &_rawValues[0]; };
    int8_t* getSegmentedValues() { return &_pinSegment[0]; };

protected:
    void read();
    void process();
    bool approxEquals(int32_t ref, int32_t val);

protected:
    CalibrationConfig& _calibrationConfig;
    uint8_t _pin[NB_PIN] = {A0, A1, A2, A3, A6, A7, A8, A9};
    int8_t _pinSegment[NB_PIN] = {	PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG,
                                    PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG };
    int8_t _lastPinSegment[NB_PIN] = {	PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG,
                                        PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG };
    unsigned long _lastTick[NB_PIN] = {};
    int _rawValues[NB_PIN] = {0};
};

#endif