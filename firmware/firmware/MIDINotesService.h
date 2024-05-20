#ifndef MIDI_NOTES_SERVICE_H
#define MIDI_NOTES_SERVICE_H

#include "AMIDIInterface.h"
#include "HardwareInterface.h"
#include "MIDIConfig.h"

class MIDINotesService {
public:
    MIDINotesService(AMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, HardwareInterface& hwInterface);
    virtual ~MIDINotesService() = default;

    void setup() {};
    void loop();

private:
    AMIDIInterface& _usbMidiInterface;
    MidiConfig& _midiConfig;
    HardwareInterface& _hwInterface;

    pdlbrd_key_t _currentKey;
};

#endif