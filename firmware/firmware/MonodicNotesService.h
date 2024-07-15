#ifndef MONODIC_NOTES_SERVICE_H
#define MONODIC_NOTES_SERVICE_H

#include "MIDINotesService.h"
#include "IMIDIInterface.h"
#include "IHardwareInterface.h"
#include "MIDIConfig.h"
#include "pdlbrdkeys.h"

class MonodicNotesService : public MIDINotesService {
public:
    MonodicNotesService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IHardwareInterface& hwInterface);
    virtual ~MonodicNotesService() = default;

    virtual void loop();

private:
    pdlbrd_key_t _currentKey;
};

#endif