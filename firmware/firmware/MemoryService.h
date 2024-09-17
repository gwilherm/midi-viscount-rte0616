#ifndef MEMORY_SERVICE_H
#define MEMORY_SERVICE_H

#include "CalibrationConfig.h"
#include "IEEPROMInterface.h"
#include "MIDIConfig.h"
#include "IMemoryService.h"

class MemoryService : public IMemoryService
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
    void update(const uint8_t* pData, int size, int offset = 0);

protected:
    IEEPROMInterface& _eepromInterface;
    MidiConfig& _midiConfig;
    CalibrationConfig& _calibrationConfig;
    int _fwVer[3];
};

#endif