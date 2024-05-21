#ifndef MIDI_NOTES_SERVICE_H
#define MIDI_NOTES_SERVICE_H

#include "AMIDIInterface.h"
#include "AHardwareInterface.h"
#include "MIDIConfig.h"
#include "pdlbrdkeys.h"

class MIDINotesService {
public:
    MIDINotesService(AMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, AHardwareInterface& hwInterface);
    virtual ~MIDINotesService() = default;

    void setup() {};
    void loop();

private:
    AMIDIInterface& _usbMidiInterface;
    MidiConfig& _midiConfig;
    AHardwareInterface& _hwInterface;

    pdlbrd_key_t _currentKey;
};

#endif