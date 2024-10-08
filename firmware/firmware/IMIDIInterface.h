#ifndef A_MIDI_INTERFACE_H
#define A_MIDI_INTERFACE_H

#include <Arduino.h>

#define MIDI_MAXVAL 127

class IMIDIInterface {
public:
    typedef enum {
        NOTE_OFF = 0,
        NOTE_ON = 1
    } note_state_t;

    typedef enum {
        CC_EXPRESSION = 11,
        CC_SUSTAIN = 64,
        CC_ALL_NOTES_OFF = 123
    } control_change_t;

public:
    virtual void sendSysEx(unsigned inLength,
                          const uint8_t* inArray) = 0;

    virtual void setHandleSystemExclusive(void (*fptr)(uint8_t * array, unsigned size)) = 0;

    virtual void sendNote(note_state_t state,
                          uint8_t inNoteNumber,
                          uint8_t inChannel) = 0;

    virtual void sendCC(uint8_t inControlNumber,
                        uint8_t inControlValue,
                        uint8_t inChannel) = 0;
};

#endif