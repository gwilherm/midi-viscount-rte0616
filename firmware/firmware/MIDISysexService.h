#ifndef SYSEX_SERVICE_H
#define SYSEX_SERVICE_H

#include <Arduino.h>

#include "IMemoryService.h"
#include "MIDIConfig.h"
#include "CalibrationConfig.h"
#include "IMIDIInterface.h"

typedef enum {
	CMD_CONFIGURATION = 1,
	CMD_CALIBRATION,
	CMD_MEASURES,
	CMD_MEASURES_REQUEST,
    CMD_FACTORY_RESET = 0x7F
} pdlbrd_sysex_cmd_t;

typedef enum {
	MEASURES_OFF = 1,
	MEASURES_ON
} pdlbrd_measures_request_t;

class MIDISysexService
{
public:
    MIDISysexService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, CalibrationConfig& calibrationConfig, IMemoryService& memoryService);
    virtual ~MIDISysexService() = default;

public:
    void handleSysEx(uint8_t* array, unsigned size);
    void sendMeasures(int* val, int nbVal);
    inline bool shouldSendMeasures() { return _shouldSendMeasures; };

protected:
    void initSysexIdentResponse();
    void handleMeasuresRequest(const pdlbrd_measures_request_t measuresRequest);
    void handleGetConfiguration();
    void handleStoreConfiguration();
    void handleSetConfiguration(uint8_t* data);
    void handleGetCalibration();
    void handleStoreCalibration();
    void handleSetCalibration(uint8_t* data);
    void handleFactoryReset();
    void handleCommand(const pdlbrd_sysex_cmd_t cmd, uint8_t* data, int data_size);

private:
    IMIDIInterface& _usbMidiInterface;
    MidiConfig& _midiConfig;
    CalibrationConfig& _calibrationConfig;
    IMemoryService& _memoryService;
    bool _shouldSendMeasures;
};

#endif