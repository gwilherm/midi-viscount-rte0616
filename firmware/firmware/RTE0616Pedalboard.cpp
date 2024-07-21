#include "RTE0616Pedalboard.h"

#include <Arduino.h>
#include "EEPROMInterface.h"
#include "CalibrationConfig.h"
#include "HardwareInterface.h"
#include "MIDIConfig.h"
#include "MIDINotesServiceManager.h"
#include "MIDISysexService.h"
#include "MemoryService.h"
#include "MonodicNotesService.h"
#include "ExtraPedalInterface.h"
#include "PolyphonicNotesService.h"
#include "USBMIDIInterface.h"

MidiConfig RTE0616Pedalboard::_midiConfig;
CalibrationConfig RTE0616Pedalboard::_calibrationConfig;

HardwareInterface RTE0616Pedalboard::_hwInterface(RTE0616Pedalboard::_calibrationConfig);
ExtraPedalInterface RTE0616Pedalboard::_extraPedalInterface;
USBMIDIInterface RTE0616Pedalboard::_midiInterface;
EEPROMInterface RTE0616Pedalboard::_eepromInterface;

MemoryService RTE0616Pedalboard::_memService(_eepromInterface, _midiConfig, _calibrationConfig);
MIDISysexService RTE0616Pedalboard::_sysexService(RTE0616Pedalboard::_midiInterface, RTE0616Pedalboard::_midiConfig, RTE0616Pedalboard::_calibrationConfig, RTE0616Pedalboard::_memService);
MonodicNotesService RTE0616Pedalboard::_monoNotesService(RTE0616Pedalboard::_midiInterface, RTE0616Pedalboard::_midiConfig, RTE0616Pedalboard::_hwInterface);
PolyphonicNotesService RTE0616Pedalboard::_polyNotesService(RTE0616Pedalboard::_midiInterface, RTE0616Pedalboard::_midiConfig, RTE0616Pedalboard::_hwInterface);
MIDINotesServiceManager RTE0616Pedalboard::_notesServiceManager(RTE0616Pedalboard::_midiConfig, RTE0616Pedalboard::_midiInterface, RTE0616Pedalboard::_monoNotesService, RTE0616Pedalboard::_polyNotesService);
MIDICCService RTE0616Pedalboard::_ccService(RTE0616Pedalboard::_midiInterface, RTE0616Pedalboard::_midiConfig, RTE0616Pedalboard::_extraPedalInterface);

RTE0616Pedalboard::RTE0616Pedalboard()
{}

void RTE0616Pedalboard::setup()
{
    _memService.restore();
    _hwInterface.setup();
    _extraPedalInterface.setup();
    _midiInterface.setup();
    _ccService.setup();
    _notesServiceManager.setup();

    // No std::bind so lambda.
    _midiInterface.setHandleSystemExclusive([&_sysexService](uint8_t* array, unsigned size) {
        _sysexService.handleSysEx(array, size);
    });
}

void RTE0616Pedalboard::loop()
{
    _hwInterface.loop();
    _extraPedalInterface.loop();
    _ccService.loop();
    _notesServiceManager.loop();
    
    if (_sysexService.shouldSendMeasures())
        _sysexService.sendMeasures(_hwInterface.getRawValues(), NB_PIN);

    _midiInterface.loop();
}

