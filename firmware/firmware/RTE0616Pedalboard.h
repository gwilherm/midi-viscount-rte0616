
#ifndef RTE0616_PEDALBOARD_H
#define RTE0616_PEDALBOARD_H

#include "CalibrationConfig.h"
#include "EEPROMInterface.h"
#include "MIDICCService.h"
#include "MIDIConfig.h"
#include "IMIDINotesService.h"
#include "MIDINotesServiceManager.h"
#include "MIDISysexService.h"
#include "HardwareInterface.h"
#include "MemoryService.h"
#include "MonodicNotesService.h"
#include "ExtraPedalInterface.h"
#include "PolyphonicNotesService.h"
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
    static ExtraPedalInterface _extraPedalInterface;
    static USBMIDIInterface _midiInterface;
    static EEPROMInterface _eepromInterface;
    static MIDISysexService _sysexService;
    static MonodicNotesService _monoNotesService;
    static PolyphonicNotesService _polyNotesService; 
    static MIDINotesServiceManager _notesServiceManager;
    static MIDICCService _ccService;
    static MemoryService _memService;
};

#endif