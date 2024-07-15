#ifndef POLYPHONIC_NOTES_SERVICE_H
#define POLYPHONIC_NOTES_SERVICE_H

#include "MIDINotesService.h"
#include "IMIDIInterface.h"
#include "IHardwareInterface.h"
#include "MIDIConfig.h"
#include "pdlbrdkeys.h"

class PolyphonicNotesService : public MIDINotesService {
public:
    PolyphonicNotesService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IHardwareInterface& hwInterface);
    virtual ~PolyphonicNotesService() = default;

    virtual void loop();

private:
    int8_t _lastPinSegment[NB_PIN];
};

#endif