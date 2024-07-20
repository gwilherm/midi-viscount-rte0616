#ifndef POLYPHONIC_NOTES_SERVICE_H
#define POLYPHONIC_NOTES_SERVICE_H

#include "IMIDINotesService.h"
#include "IMIDIInterface.h"
#include "IHardwareInterface.h"
#include "MIDIConfig.h"
#include "pdlbrdkeys.h"

#include <Arduino.h>

class PolyphonicNotesService : public IMIDINotesService {
public:
    PolyphonicNotesService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IHardwareInterface& hwInterface);
    virtual ~PolyphonicNotesService() = default;

    virtual void setup() {};
    virtual void loop();
    virtual void reset();

private:
    IMIDIInterface& _usbMidiInterface;
    MidiConfig& _midiConfig;
    IHardwareInterface& _hwInterface;
    int8_t _lastPinSegment[NB_PIN];
};

#endif