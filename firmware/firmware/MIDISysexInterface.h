#ifndef SYSEX_INTERFACE_H
#define SYSEX_INTERFACE_H

#include <Arduino.h>
#include <USB-MIDI.h>

#include "MIDIConfig.h"
#include "CalibrationConfig.h"

typedef enum {
	CMD_CONFIGURATION = 1,
	CMD_CALIBRATION,
	CMD_MEASURES,
	CMD_MEASURES_REQUEST,
	CMD_MAX
} pdlbrd_sysex_cmd_t;

typedef enum {
	MEASURES_OFF = 1,
	MEASURES_ON
} pdlbrd_measures_request_t;

class MIDISysexInterface
{
public:
    MIDISysexInterface(MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport>& usbMidiInterface, MidiConfig& midiConfig, CalibrationConfig& calibrationConfig);
    virtual ~MIDISysexInterface() = default;

public:
    void handleSysEx(uint8_t* array, unsigned size);
    void sendMesures(int* val, int nbVal);
    inline bool shouldSendMeasures() { return _shouldSendMeasures; };

protected:
    void initSysexIdentResponse();
    void handleMeasuresRequest(const pdlbrd_measures_request_t measuresRequest);
    void handleGetConfiguration();
    void handleSetConfiguration(uint8_t* data);
    void handleGetCalibration();
    void handleSetCalibration(uint8_t* data);
    void handleCommand(const pdlbrd_sysex_cmd_t cmd, uint8_t* data, int data_size);

private:
    MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport>& _usbMidiInterface;
    MidiConfig& _midiConfig;
    CalibrationConfig& _calibrationConfig;
    bool _shouldSendMeasures;
};

#endif