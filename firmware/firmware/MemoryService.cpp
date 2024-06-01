#include "MemoryService.h"
#include "IEEPROMInterface.h"
#include "MIDIConfig.h"
#include "CalibrationConfig.h"

#define MANUFACTURER_ID 0x31 	// Viscount Manufacturer ID
#define PRODUCT_ID_MSB  0x06
#define PRODUCT_ID_LSB  0x16	// for RTE0616

struct device_info_store_s {
    device_info_store_s() {
        char* fwVersionStr = strdup(FW_VERSION);
    	int tmp = atoi(strtok(fwVersionStr, "."));
        v[0] = tmp & 0xFF;
    	tmp = atoi(strtok(NULL, "."));
        v[1] = tmp & 0xFF;
        tmp = atoi(strtok(NULL, "."));
        v[2] = (tmp >> 8) & 0xFF;
        v[3] = tmp & 0xFF;
    };
    uint8_t m = MANUFACTURER_ID;
    uint8_t p[2] = {PRODUCT_ID_MSB,PRODUCT_ID_LSB};
    uint8_t v[4];
};

typedef union device_info_store_u
{
    device_info_store_u() : info() {};
    struct device_info_store_s info;
    uint8_t bytes[sizeof(struct device_info_store_s)];
} device_info_store_t;

typedef union midi_config_store_u
{
    midi_config_store_u() {};
    midi_config_store_u(MidiConfig& midiConfig) : cfg(midiConfig.getChannel(), midiConfig.getOctave()) {};
    struct midi_config_store_s {
        midi_config_store_s(uint8_t c, uint8_t o) : channel(c), octave(o) {};
        uint8_t channel;
        uint8_t octave;
    } cfg;
    uint8_t bytes[sizeof(struct midi_config_store_s)];
} midi_config_store_t;

typedef union calibration_store_u
{
    calibration_store_u() {};
    calibration_store_u(CalibrationConfig& calConfig) : cal(calConfig.getMargin()) {
        for (int i = 0; i < NB_VSEG; i++) cal.vseg[i] = calConfig.getVSeg(i);
    };
    struct calibration_store_s {
        calibration_store_s(uint8_t m) : margin(m) {};
        uint16_t margin;
        uint16_t vseg[NB_VSEG];
    } cal;
    uint8_t bytes[sizeof(struct calibration_store_s)];
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
    device_info_store_t dev;

    for (int i = 0; i < sizeof(device_info_store_t); i++)
        _eepromInterface.update(offset++, dev.bytes[i]);

    return offset;
}

int MemoryService::updateMidiConfig()
{
    int offset = sizeof(device_info_store_t);
    midi_config_store_t cfg(_midiConfig);

    for (int i = 0; i < sizeof(midi_config_store_t); i++)
        _eepromInterface.update(offset++, cfg.bytes[i]);

    return offset;
}

int MemoryService::updateCalibration()
{
    int offset = 7 + sizeof(midi_config_store_t);

    calibration_store_t cal(_calibrationConfig);
    for (int i = 0; i < sizeof(calibration_store_t);  i++)
        _eepromInterface.update(offset++, cal.bytes[i]);

    return offset;
}

void MemoryService::store()
{
    int offset = 0;

    offset = updateDeviceInfo();
    offset = updateMidiConfig();
    offset = updateCalibration();
}

void MemoryService::restore()
{
    midi_config_store_t cfg;
    calibration_store_t cal;

    int offset = 0;

    if (readDeviceInfo(offset))
    {
        for (int i = 0; i < sizeof(midi_config_store_t); i++)
            cfg.bytes[i] = _eepromInterface.read(offset++);

        for (int i = 0; i < sizeof(calibration_store_t);  i++)
            cal.bytes[i] = _eepromInterface.read(offset++);

        _midiConfig.setChannel(cfg.cfg.channel);
        _midiConfig.setOctave(cfg.cfg.octave);

        _calibrationConfig.setMargin(cal.cal.margin);
        for (int i = 0; i < NB_VSEG; i++)
            _calibrationConfig.setVSeg(i, cal.cal.vseg[i]);
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