#ifndef MIDI_NOTES_SERVICE_MANAGER_H
#define MIDI_NOTES_SERVICE_MANAGER_H

#include "IMIDIInterface.h"
#include "IHardwareInterface.h"
#include "MIDIConfig.h"
#include "IMIDINotesService.h"
#include "pdlbrdkeys.h"

class MIDINotesServiceManager {
public:
    MIDINotesServiceManager(MidiConfig& midiConfig, IMIDIInterface& midiInterface, IMIDINotesService& monoNotesService, IMIDINotesService& polyNotesService);

    void setup();
    void loop();
    IMIDINotesService* noteService(MidiConfig::keyboard_mode_t mode);

protected:
    MidiConfig& _midiConfig;
    IMIDIInterface& _midiInterface;
    IMIDINotesService& _monoNotesService;
    IMIDINotesService& _polyNotesService;
    MidiConfig::keyboard_mode_t _currentMode;
};

#endif