#ifndef USB_MIDI_INTERFACE_H
#define USB_MIDI_INTERFACE_H

#include "IMIDIInterface.h"

#include <USB-MIDI.h>

class USBMIDIInterface : public IMIDIInterface {
public:
    USBMIDIInterface();
    virtual ~USBMIDIInterface() = default;

    void setup();
    void loop();

    virtual void sendSysEx(unsigned inLength,
                          const uint8_t* inArray);

    virtual void setHandleSystemExclusive(void (*fptr)(uint8_t * array, unsigned size));

    virtual void sendNoteOn(uint8_t inNoteNumber,
                            uint8_t inChannel);

    virtual void sendNoteOff(uint8_t inNoteNumber,
                            uint8_t inChannel);

    virtual void sendCC(uint8_t inControlNumber,
                        uint8_t inControlValue,
                        uint8_t inChannel);
protected:
    USBMIDI_NAMESPACE::usbMidiTransport _usbMidi;
    MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport> _usbMidiInterface;
};

#endif