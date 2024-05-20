
#ifndef RTE0616_PEDALBOARD_H
#define RTE0616_PEDALBOARD_H

#include "CalibrationConfig.h"
#include "MIDIConfig.h"
#include "MIDISysexInterface.h"
#include "HardwareInterface.h"
#include "USBMIDIInterface.h"

class RTE0616Pedalboard
{
public:
    RTE0616Pedalboard();
    virtual ~RTE0616Pedalboard() = default;

    void setup();
    void loop();

protected:
    void process(int8_t* pinSegment);

public:
    static MidiConfig _midiConfig;
    static CalibrationConfig _calibrationConfig;
    static HardwareInterface _hwInterface;
    static USBMIDIInterface _midiInterface;
    static MIDISysexInterface _sysexInterface;
};

#endif