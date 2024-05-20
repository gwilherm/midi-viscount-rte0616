
#ifndef RTE0616_PEDALBOARD_H
#define RTE0616_PEDALBOARD_H

#include "CalibrationConfig.h"
#include "MIDIConfig.h"
#include "MIDINotesService.h"
#include "MIDISysexService.h"
#include "HardwareInterface.h"
#include "USBMIDIInterface.h"

class RTE0616Pedalboard
{
public:
    RTE0616Pedalboard();
    virtual ~RTE0616Pedalboard() = default;

    void setup();
    void loop();

public:
    static MidiConfig _midiConfig;
    static CalibrationConfig _calibrationConfig;
    static HardwareInterface _hwInterface;
    static USBMIDIInterface _midiInterface;
    static MIDISysexService _sysexService;
    static MIDINotesService _notesService;
};

#endif