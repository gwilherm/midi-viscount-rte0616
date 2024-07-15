#ifndef MIDI_NOTES_SERVICE_H
#define MIDI_NOTES_SERVICE_H

#include "IMIDIInterface.h"
#include "IHardwareInterface.h"
#include "MIDIConfig.h"
#include "pdlbrdkeys.h"

class MIDINotesService {
public:
    MIDINotesService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IHardwareInterface& hwInterface);

    void setup() {};
    virtual void loop() = 0;

    void sendNote(IMIDIInterface::note_state_t state, pdlbrd_key_t key);

protected:
    IMIDIInterface& _usbMidiInterface;
    MidiConfig& _midiConfig;
    IHardwareInterface& _hwInterface;
};

#endif