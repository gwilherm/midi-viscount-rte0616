#ifndef I_MIDI_NOTES_SERVICE_H
#define I_MIDI_NOTES_SERVICE_H

#include "IMIDIInterface.h"
#include "MIDIConfig.h"
#include "pdlbrdkeys.h"

class IMIDINotesService {
public:
    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual void reset() = 0;

    static void sendNote(IMIDIInterface& midiInterface, MidiConfig& midiConfig, IMIDIInterface::note_state_t state, pdlbrd_key_t key);
};

#endif