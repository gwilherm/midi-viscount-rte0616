#ifndef MIDI_CC_SERVICE_H
#define MIDI_CC_SERVICE_H

#include "IMIDIInterface.h"
#include "MIDIConfig.h"
#include "IPedalInterface.h"
#include "pdlbrdkeys.h"

class MIDICCService {
public:
    MIDICCService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IPedalInterface& pedalInterface);
    virtual ~MIDICCService() = default;

    void setup() {};
    void loop();

private:
    IMIDIInterface& _usbMidiInterface;
    MidiConfig& _midiConfig;
    IPedalInterface& _pedalInterface;

    uint8_t _lastExpressionValue;
    bool _lastSustainState;
};

#endif