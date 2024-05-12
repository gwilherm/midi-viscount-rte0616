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

    int getChannel() { return _channel; };
    void setChannel(int c) { _channel = c; };
    int getOctave() { return _octave; };
    void setOctave(int o) { _octave = o; };

private:
    uint8_t _channel;
    uint8_t _octave;
};
#endif