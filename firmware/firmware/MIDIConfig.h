#ifndef MIDI_CONFIG_H
#define MIDI_CONFIG_H

#define DEFAULT_MIDI_CHANNEL 1
#define DEFAULT_OCTAVE 3

#include <Arduino.h>

class MidiConfig
{
public:
    typedef enum {
        MONODIC_UP = 0,
        POLYPHONIC = 1,
        KEYBOARD_MODE_MAX = POLYPHONIC
    } keyboard_mode_t;

public:
    MidiConfig() : _channel(DEFAULT_MIDI_CHANNEL), _octave(DEFAULT_OCTAVE), _kbd_mode(MONODIC_UP) {};
    MidiConfig(uint8_t channel, uint8_t octave, keyboard_mode_t mode) : _channel(channel), _octave(octave), _kbd_mode(mode) {};
    virtual ~MidiConfig() = default;

    inline uint8_t getChannel() { return _channel; };
    inline void setChannel(uint8_t c) { _channel = c; };
    inline uint8_t getOctave() { return _octave; };
    inline void setOctave(uint8_t o) { _octave = o; };
    inline keyboard_mode_t getKeyboardMode() { return _kbd_mode; };
    inline void setKeyboardMode(keyboard_mode_t m) { _kbd_mode = m; };
    inline void reset();

private:
    uint8_t _channel;
    uint8_t _octave;
    keyboard_mode_t _kbd_mode;
};

void MidiConfig::reset()
{
    _channel = DEFAULT_MIDI_CHANNEL;
    _octave = DEFAULT_OCTAVE;
}

#endif