#include "MemoryService.h"
#include "IEEPROMInterface.h"
#include "MIDIConfig.h"
#include "CalibrationConfig.h"
#include <cstdint>

#define MANUFACTURER_ID 0x31 	// Viscount Manufacturer ID
#define PRODUCT_ID_MSB  0x06
#define PRODUCT_ID_LSB  0x16	// for RTE0616

typedef struct {
    uint8_t manu_id;
    uint8_t prod_id[2];
    uint8_t fw_ver[4];
} device_info_store_t;

typedef struct {
    uint8_t channel;
    uint8_t octave;
} midi_config_store_t;

typedef struct {
    uint16_t margin;
    uint16_t vseg[NB_VSEG];
} calibration_store_t;

MemoryService::MemoryService(IEEPROMInterface& eepromInterface, MidiConfig& midiConfig, CalibrationConfig& calibrationConfig):
    _eepromInterface(eepromInterface),
    _midiConfig(midiConfig),
    _calibrationConfig(calibrationConfig)
{}

bool MemoryService::readDeviceInfo(int& offset)
{
    bool ret = true;

    ret &= (MANUFACTURER_ID == _eepromInterface.read(offset++));
    ret &= (PRODUCT_ID_MSB  == _eepromInterface.read(offset++));
    ret &= (PRODUCT_ID_LSB  == _eepromInterface.read(offset++));

    offset += 4;

    return ret;
}

int MemoryService::updateDeviceInfo()
{
    int offset = 0;
    device_info_store_t dev = {
        .manu_id = MANUFACTURER_ID,
        .prod_id = { PRODUCT_ID_MSB, PRODUCT_ID_LSB }
    };

    char* fwVersionStr = strdup(FW_VERSION);
    int tmp = atoi(strtok(fwVersionStr, "."));
    dev.fw_ver[0] = tmp & 0xFF;
    tmp = atoi(strtok(NULL, "."));
    dev.fw_ver[1] = tmp & 0xFF;
    tmp = atoi(strtok(NULL, "."));
    dev.fw_ver[2] = (tmp >> 8) & 0xFF;
    dev.fw_ver[3] = tmp & 0xFF;

    uint8_t* ptr = (uint8_t*)&dev;
    for (int i = 0; i < sizeof(device_info_store_t); i++)
        _eepromInterface.update(offset++, *ptr++);

    return offset;
}

int MemoryService::updateMidiConfig()
{
    int offset = sizeof(device_info_store_t);
    midi_config_store_t cfg = {
        .channel = _midiConfig.getChannel(),
        .octave = _midiConfig.getOctave()
    };

    uint8_t* ptr = (uint8_t*)&cfg;
    for (int i = 0; i < sizeof(cfg); i++)
        _eepromInterface.update(offset++, *ptr++);

    return offset;
}

int MemoryService::updateCalibration()
{
    int offset = 7 + sizeof(midi_config_store_t);

    calibration_store_t cal = {
        .margin = _calibrationConfig.getMargin(),
        .vseg = {
            _calibrationConfig.getVSeg(0),
            _calibrationConfig.getVSeg(1),
            _calibrationConfig.getVSeg(2),
            _calibrationConfig.getVSeg(3),
        }
    };

    uint8_t* ptr = (uint8_t*)&cal;
    for (int i = 0; i < sizeof(calibration_store_t);  i++)
        _eepromInterface.update(offset++, *ptr++);

    return offset;
}

void MemoryService::store()
{
    updateDeviceInfo();
    updateMidiConfig();
    updateCalibration();
}

void MemoryService::restore()
{
    midi_config_store_t cfg;
    calibration_store_t cal;
    uint8_t* ptr;

    int offset = 0;

    if (readDeviceInfo(offset))
    {
        ptr = (uint8_t*)&cfg;
        for (int i = 0; i < sizeof(midi_config_store_t); i++)
            *ptr++ = _eepromInterface.read(offset++);

        ptr = (uint8_t*)&cal;
        for (int i = 0; i < sizeof(calibration_store_t);  i++)
            *ptr++ = _eepromInterface.read(offset++);

        _midiConfig.setChannel(cfg.channel);
        _midiConfig.setOctave(cfg.octave);

        _calibrationConfig.setMargin(cal.margin);
        for (int i = 0; i < NB_VSEG; i++)
            _calibrationConfig.setVSeg(i, cal.vseg[i]);
    }
    else {
        factoryReset();
    }
}

void MemoryService::factoryReset()
{
    _midiConfig.reset();
    _calibrationConfig.reset();
    store();
}