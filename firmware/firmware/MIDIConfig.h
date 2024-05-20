#ifndef MIDI_CONFIG_H
#define MIDI_CONFIG_H

#define DEFAULT_MIDI_CHANNEL 1
#define DEFAULT_OCTAVE 3

#include <Arduino.h>

class MidiConfig
{
public:
    MidiConfig() : _channel(DEFAULT_MIDI_CHANNEL), _octave(DEFAULT_OCTAVE) {}
    virtual ~MidiConfig() = default;

    inline uint8_t getChannel() { return _channel; };
    inline void setChannel(uint8_t c) { _channel = c; };
    inline uint8_t getOctave() { return _octave; };
    inline void setOctave(uint8_t o) { _octave = o; };
    inline void reset();

private:
    uint8_t _channel;
    uint8_t _octave;
};

void MidiConfig::reset()
{
    _channel = DEFAULT_MIDI_CHANNEL;
    _octave = DEFAULT_OCTAVE;
}

#endif