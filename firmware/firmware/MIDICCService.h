#ifndef MIDI_CC_SERVICE_H
#define MIDI_CC_SERVICE_H

#include "IMIDIInterface.h"
#include "MIDIConfig.h"
#include "IExtraPedalInterface.h"
#include "pdlbrdkeys.h"

class MIDICCService {
public:
    MIDICCService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IExtraPedalInterface& pedalInterface);
    virtual ~MIDICCService() = default;

    void setup() {};
    void loop();

private:
    IMIDIInterface& _usbMidiInterface;
    MidiConfig& _midiConfig;
    IExtraPedalInterface& _extraPedalInterface;

    uint8_t _lastExpressionValue;
    bool _lastSustainState;
};

#endif