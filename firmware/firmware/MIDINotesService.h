#ifndef MIDI_NOTES_SERVICE_H
#define MIDI_NOTES_SERVICE_H

#include "IMIDIInterface.h"
#include "IHardwareInterface.h"
#include "MIDIConfig.h"
#include "pdlbrdkeys.h"

class MIDINotesService {
public:
    MIDINotesService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IHardwareInterface& hwInterface);
    virtual ~MIDINotesService() = default;

    void setup() {};
    void loop();

private:
    IMIDIInterface& _usbMidiInterface;
    MidiConfig& _midiConfig;
    IHardwareInterface& _hwInterface;

    pdlbrd_key_t _currentKey;
};

#endif