
#ifndef RTE0616_PEDALBOARD_H
#define RTE0616_PEDALBOARD_H

#include "CalibrationConfig.h"
#include "MIDIConfig.h"
#include "MIDISysexInterface.h"

class RTE0616Pedalboard
{
public:
    RTE0616Pedalboard();
    virtual ~RTE0616Pedalboard() = default;

    void setup();
    void loop();

protected:
    void process(int* val);
    bool approxEquals(int32_t ref, int32_t val);

public:
};

#endif