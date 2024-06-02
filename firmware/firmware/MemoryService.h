#ifndef MEMORY_SERVICE_H
#define MEMORY_SERVICE_H

#include "CalibrationConfig.h"
#include "IEEPROMInterface.h"
#include "MIDIConfig.h"

class MemoryService
{
public:
    MemoryService(IEEPROMInterface& eepromInterface, MidiConfig& midiConfig, CalibrationConfig& calibrationConfig);
    virtual ~MemoryService() = default;

    void restore();
    void store();
    void factoryReset();

    void updateMidiConfig();
    void updateCalibration();

protected:
    void updateDeviceInfo();
    bool checkDeviceInfo();

protected:
    IEEPROMInterface& _eepromInterface;
    MidiConfig& _midiConfig;
    CalibrationConfig& _calibrationConfig;
};

#endif