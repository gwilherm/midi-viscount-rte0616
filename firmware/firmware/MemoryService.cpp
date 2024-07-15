#include "MemoryService.h"
#include "IEEPROMInterface.h"
#include "MIDIConfig.h"
#include "CalibrationConfig.h"

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
    uint8_t mode;
} midi_config_store_t;

typedef struct {
    uint16_t margin;
    uint16_t vseg[NB_VSEG];
} calibration_store_t;

#pragma pack(push, 1)
typedef struct {
    device_info_store_t dev;
    midi_config_store_t cfg;
    calibration_store_t cal;
} memory_storage_t ;
#pragma pack(pop)


MemoryService::MemoryService(IEEPROMInterface& eepromInterface, MidiConfig& midiConfig, CalibrationConfig& calibrationConfig):
    _eepromInterface(eepromInterface),
    _midiConfig(midiConfig),
    _calibrationConfig(calibrationConfig)
{}

bool MemoryService::checkDeviceInfo()
{
    bool ret = true;
    int offset = 0;

    ret &= (MANUFACTURER_ID == _eepromInterface.read(offset++));
    ret &= (PRODUCT_ID_MSB  == _eepromInterface.read(offset++));
    ret &= (PRODUCT_ID_LSB  == _eepromInterface.read(offset++));

    return ret;
}

void MemoryService::update(const uint8_t* pData, int size, int offset)
{
    for (int i = 0; i < size; i++)
        _eepromInterface.update(offset++, *pData++);
}

void MemoryService::updateDeviceInfo()
{
    int offset = offsetof(memory_storage_t, dev);
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

    update((uint8_t*)&dev, sizeof(device_info_store_t), offset);
}

void MemoryService::updateMidiConfig()
{
    int offset = offsetof(memory_storage_t, cfg);

    midi_config_store_t cfg = {
        .channel = _midiConfig.getChannel(),
        .octave = _midiConfig.getOctave(),
        .mode = _midiConfig.getKeyboardMode()
    };

    update((uint8_t*)&cfg, sizeof(midi_config_store_t), offset);
}

void MemoryService::updateCalibration()
{
    int offset = offsetof(memory_storage_t, cal);

    calibration_store_t cal = {
        .margin = _calibrationConfig.getMargin(),
        .vseg = {
            _calibrationConfig.getVSeg(0),
            _calibrationConfig.getVSeg(1),
            _calibrationConfig.getVSeg(2),
            _calibrationConfig.getVSeg(3),
        }
    };

    update((uint8_t*)&cal, sizeof(calibration_store_t), offset);
}

void MemoryService::store()
{
    updateDeviceInfo();
    updateMidiConfig();
    updateCalibration();
}

void MemoryService::restore()
{
    memory_storage_t mem;
    uint8_t* ptr = (uint8_t*)&mem;

    if (checkDeviceInfo())
    {
        for (int i = 0; i < sizeof(memory_storage_t); i++)
            *ptr++ = _eepromInterface.read(i);

        _midiConfig.setChannel(mem.cfg.channel);
        _midiConfig.setOctave(mem.cfg.octave);
        _midiConfig.setKeyboardMode((MidiConfig::keyboard_mode_t) mem.cfg.mode);

        _calibrationConfig.setMargin(mem.cal.margin);
        for (int i = 0; i < NB_VSEG; i++)
            _calibrationConfig.setVSeg(i, mem.cal.vseg[i]);
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