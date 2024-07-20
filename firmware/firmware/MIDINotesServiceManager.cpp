#include "MIDINotesServiceManager.h"
#include "IMIDIInterface.h"
#include "IMIDINotesService.h"
#include "MIDIConfig.h"

MIDINotesServiceManager::MIDINotesServiceManager(MidiConfig& midiConfig, IMIDIInterface& midiInterface, IMIDINotesService& monoNotesService, IMIDINotesService& polyNotesService) :
    _midiInterface(midiInterface),
    _midiConfig(midiConfig),
    _monoNotesService(monoNotesService),
    _polyNotesService(polyNotesService),
    _currentMode(MidiConfig::MONODIC_UP)
{
}

// To be called after memory restore
void MIDINotesServiceManager::setup()
{
    _currentMode = _midiConfig.getKeyboardMode();

    _monoNotesService.setup();
    _polyNotesService.setup();
}

IMIDINotesService* MIDINotesServiceManager::noteService(MidiConfig::keyboard_mode_t mode)
{
    switch (mode) {
    case MidiConfig::MONODIC_UP: return &_monoNotesService;
    case MidiConfig::POLYPHONIC: return &_polyNotesService;
    }

    return nullptr;
}

void MIDINotesServiceManager::loop()
{
    if (_currentMode != _midiConfig.getKeyboardMode())
    {
        noteService(_currentMode)->reset();
        _midiInterface.sendCC(IMIDIInterface::CC_ALL_NOTES_OFF, 0, _midiConfig.getChannel());
        _currentMode = _midiConfig.getKeyboardMode();
    }

    noteService(_currentMode)->loop();
}
