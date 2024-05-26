#include "MemoryService.h"
#include "IEEPROMInterface.h"
#include "MIDIConfig.h"
#include "CalibrationConfig.h"

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

void MemoryService::store()
{
    midi_config_store_t cfg(_midiConfig);
    calibration_store_t cal(_calibrationConfig);

    int offset = 0;

    for (int i = 0; i < sizeof(midi_config_store_t); i++)
        _eepromInterface.update(offset++, cfg.bytes[i]);

    for (int i = 0; i < sizeof(calibration_store_t);  i++)
        _eepromInterface.update(offset++, cal.bytes[i]);
}

void MemoryService::restore()
{
    midi_config_store_t cfg;
    calibration_store_t cal;

    int offset = 0;
    
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

void MemoryService::factoryReset()
{
    _midiConfig.reset();
    _calibrationConfig.reset();
    store();
}