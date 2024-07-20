#include "USBMIDIInterface.h"

USBMIDIInterface::USBMIDIInterface() :
    _usbMidi(0),
    _usbMidiInterface(_usbMidi)
{}

void USBMIDIInterface::setup()
{
	_usbMidiInterface.begin(MIDI_CHANNEL_OMNI);
}

void USBMIDIInterface::loop()
{
	_usbMidiInterface.read();
}

void USBMIDIInterface::sendSysEx(unsigned inLength,
                                 const uint8_t* inArray)
{
    _usbMidiInterface.sendSysEx(inLength, inArray, false);
}

void USBMIDIInterface::setHandleSystemExclusive(void (*fptr)(uint8_t * array, unsigned size))
{
    _usbMidiInterface.setHandleSystemExclusive(fptr);
}

void USBMIDIInterface::sendNote(IMIDIInterface::note_state_t state,
                                uint8_t inNoteNumber,
                                uint8_t inChannel)
{
    if (state == NOTE_ON)
        _usbMidiInterface.sendNoteOn(inNoteNumber, MIDI_MAXVAL, inChannel);
    else
        _usbMidiInterface.sendNoteOff(inNoteNumber, 0, inChannel);
}

void USBMIDIInterface::sendCC(uint8_t inControlNumber,
                              uint8_t inControlValue,
                              uint8_t inChannel)
{
    _usbMidiInterface.sendControlChange(inControlNumber, inControlValue, inChannel);
}