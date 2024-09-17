#include "IMIDINotesService.h"

#define TONES_IN_OCTAVE 12

void IMIDINotesService::sendNote(IMIDIInterface& midiInterface, MidiConfig& midiConfig, IMIDIInterface::note_state_t state, pdlbrd_key_t key)
{
    midiInterface.sendNote(state,
                           (midiConfig.getOctave()*TONES_IN_OCTAVE) + key,
                            midiConfig.getChannel());
}
