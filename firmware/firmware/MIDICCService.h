#ifndef MIDI_CC_SERVICE_H
#define MIDI_CC_SERVICE_H

#include "AMIDIInterface.h"
#include "MIDIConfig.h"
#include "PedalInterface.h"
#include "pdlbrdkeys.h"

class MIDICCService {
public:
    MIDICCService(AMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, PedalInterface& pedalInterface);
    virtual ~MIDICCService() = default;

    void setup() {};
    void loop();

private:
    AMIDIInterface& _usbMidiInterface;
    MidiConfig& _midiConfig;
    PedalInterface& _pedalInterface;

};

#endif