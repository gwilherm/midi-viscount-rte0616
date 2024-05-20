#ifndef A_MIDI_INTERFACE_H
#define A_MIDI_INTERFACE_H

#include <Arduino.h>

class AMIDIInterface {
public:
    virtual void sendSysEx(unsigned inLength,
                          const uint8_t* inArray) = 0;

    virtual void setHandleSystemExclusive(void (*fptr)(uint8_t * array, unsigned size)) = 0;

    virtual void sendNoteOn(uint8_t inNoteNumber,
                            uint8_t inChannel) = 0;

    virtual void sendNoteOff(uint8_t inNoteNumber,
                            uint8_t inChannel) = 0;
};

#endif