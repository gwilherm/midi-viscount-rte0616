#ifndef MONODIC_NOTES_SERVICE_H
#define MONODIC_NOTES_SERVICE_H

#include "IMIDINotesService.h"
#include "IMIDIInterface.h"
#include "IHardwareInterface.h"
#include "MIDIConfig.h"
#include "pdlbrdkeys.h"

class MonodicNotesService : public IMIDINotesService {
public:
    MonodicNotesService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IHardwareInterface& hwInterface);
    virtual ~MonodicNotesService() = default;

    virtual void setup() {};
    virtual void loop();
    virtual void reset();

private:
    IMIDIInterface& _usbMidiInterface;
    MidiConfig& _midiConfig;
    IHardwareInterface& _hwInterface;
    pdlbrd_key_t _currentKey;
};

#endif