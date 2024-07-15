#include "MIDINotesService.h"

#define TONES_IN_OCTAVE 12


MIDINotesService::MIDINotesService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IHardwareInterface& hwInterface) :
    _usbMidiInterface(usbMidiInterface),
    _midiConfig(midiConfig),
    _hwInterface(hwInterface)
{}

void MIDINotesService::sendNote(IMIDIInterface::note_state_t state, pdlbrd_key_t key)
{
    _usbMidiInterface.sendNote(state,
                               (_midiConfig.getOctave()*TONES_IN_OCTAVE) + key,
                               _midiConfig.getChannel());
}
