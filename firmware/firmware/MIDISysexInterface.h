#ifndef SYSEX_INTERFACE_H
#define SYSEX_INTERFACE_H

#include "MIDIConfig.h"
#include <Arduino.h>
#include <USB-MIDI.h>

typedef enum {
	CMD_CONFIGURATION = 1,
	CMD_CALIBRATION,
	CMD_MEASURES,
	CMD_CHANGE_MODE,
	CMD_MAX
} pdlbrd_sysex_cmd_t;

typedef enum {
	MODE_STANDARD = 1,
	MODE_MEASURE,
	MODE_MAX
} pdlbrd_mode_t;

class MIDISysexInterface
{
public:
    MIDISysexInterface(MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport>& usbMidiInterface, MidiConfig& config);
    virtual ~MIDISysexInterface();

public:
    void handleSysEx(uint8_t* array, unsigned size);

protected:
    void initSysexIdentResponse();
    void handleChangeMode(const pdlbrd_mode_t mode);
    void handleGetConfiguration();
    void handleSetConfiguration(uint8_t* data);
    void handleGetCalibration();
    void handleSetCalibration(uint8_t* data);
    void handleCommand(const pdlbrd_sysex_cmd_t cmd, uint8_t* data, int data_size);
    void sendMesures(int* val, int nbVal);

private:
    MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport> _usbMidiInterface;
    MidiConfig _config;
};

#endif