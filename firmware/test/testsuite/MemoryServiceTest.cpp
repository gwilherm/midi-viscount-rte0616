#include <gtest/gtest.h>
#include "MIDIConfig.h"
#include "EEPROMInterfaceMock.h"
#include "MemoryService.h"
#include "SerialPrinter.h"
#include <cstdint>

SerialPrinter Serial;

using namespace ::testing;

class MemoryServiceTest : public Test {
public:
    MemoryServiceTest():
        _midiConfig(1, 3),
        _calibrationConfig(12, 1023, 767 ,511, 63),
        _memoryService(_eepromInterfaceMock, _midiConfig, _calibrationConfig)
    {};

protected:
    MidiConfig _midiConfig;
    CalibrationConfig _calibrationConfig;
    EEPROMInterfaceMock _eepromInterfaceMock;
    MemoryService _memoryService;
};

TEST_F(MemoryServiceTest, store)
{
    // MIDI Config
    EXPECT_CALL(_eepromInterfaceMock, update(0,1));
    EXPECT_CALL(_eepromInterfaceMock, update(1,3));

    // Calibration config
    EXPECT_CALL(_eepromInterfaceMock, update(2,12));
    EXPECT_CALL(_eepromInterfaceMock, update(3,0));
    EXPECT_CALL(_eepromInterfaceMock, update(4,255));
    EXPECT_CALL(_eepromInterfaceMock, update(5,3));
    EXPECT_CALL(_eepromInterfaceMock, update(6,255));
    EXPECT_CALL(_eepromInterfaceMock, update(7,2));
    EXPECT_CALL(_eepromInterfaceMock, update(8,255));
    EXPECT_CALL(_eepromInterfaceMock, update(9,1));
    EXPECT_CALL(_eepromInterfaceMock, update(10,63));
    EXPECT_CALL(_eepromInterfaceMock, update(11,0));

    _memoryService.store();

    // MIDI Config
    EXPECT_CALL(_eepromInterfaceMock, update(0,15));
    EXPECT_CALL(_eepromInterfaceMock, update(1,6));

    // Calibration config
    EXPECT_CALL(_eepromInterfaceMock, update(2,12));
    EXPECT_CALL(_eepromInterfaceMock, update(3,0));
    EXPECT_CALL(_eepromInterfaceMock, update(4,255));
    EXPECT_CALL(_eepromInterfaceMock, update(5,3));
    EXPECT_CALL(_eepromInterfaceMock, update(6,255));
    EXPECT_CALL(_eepromInterfaceMock, update(7,2));
    EXPECT_CALL(_eepromInterfaceMock, update(8,255));
    EXPECT_CALL(_eepromInterfaceMock, update(9,1));
    EXPECT_CALL(_eepromInterfaceMock, update(10,63));
    EXPECT_CALL(_eepromInterfaceMock, update(11,0));

    _midiConfig.setChannel(15);
    _midiConfig.setOctave(6);

    _memoryService.store();
}

TEST_F(MemoryServiceTest, restore)
{
    // MIDI Config
    EXPECT_CALL(_eepromInterfaceMock, read(0))
        .WillOnce(Return(15));
    EXPECT_CALL(_eepromInterfaceMock, read(1))
        .WillOnce(Return(6));

    // Calibration Config
    EXPECT_CALL(_eepromInterfaceMock, read(2))
        .WillOnce(Return(12));
    EXPECT_CALL(_eepromInterfaceMock, read(3))
        .WillOnce(Return(0));
    EXPECT_CALL(_eepromInterfaceMock, read(4))
        .WillOnce(Return(255));
    EXPECT_CALL(_eepromInterfaceMock, read(5))
        .WillOnce(Return(3));
    EXPECT_CALL(_eepromInterfaceMock, read(6))
        .WillOnce(Return(255));
    EXPECT_CALL(_eepromInterfaceMock, read(7))
        .WillOnce(Return(2));
    EXPECT_CALL(_eepromInterfaceMock, read(8))
        .WillOnce(Return(255));
    EXPECT_CALL(_eepromInterfaceMock, read(9))
        .WillOnce(Return(1));
    EXPECT_CALL(_eepromInterfaceMock, read(10))
        .WillOnce(Return(63));
    EXPECT_CALL(_eepromInterfaceMock, read(11))
        .WillOnce(Return(0));

    _memoryService.restore();

    EXPECT_EQ(_midiConfig.getChannel(), 15);
    EXPECT_EQ(_midiConfig.getOctave(), 6);
    EXPECT_EQ(_calibrationConfig.getMargin(), 12);
    EXPECT_EQ(_calibrationConfig.getVSeg(0), 1023);
    EXPECT_EQ(_calibrationConfig.getVSeg(1), 767);
    EXPECT_EQ(_calibrationConfig.getVSeg(2), 511);
    EXPECT_EQ(_calibrationConfig.getVSeg(3), 63);
}

TEST_F(MemoryServiceTest, factory_reset)
{
    // MIDI Config
    EXPECT_CALL(_eepromInterfaceMock, update(0,1));
    EXPECT_CALL(_eepromInterfaceMock, update(1,3));

    // Calibration Config
    EXPECT_CALL(_eepromInterfaceMock, update(2,50));
    EXPECT_CALL(_eepromInterfaceMock, update(3,0));
    EXPECT_CALL(_eepromInterfaceMock, update(4,62));
    EXPECT_CALL(_eepromInterfaceMock, update(5,3));
    EXPECT_CALL(_eepromInterfaceMock, update(6,0));
    EXPECT_CALL(_eepromInterfaceMock, update(7,2));
    EXPECT_CALL(_eepromInterfaceMock, update(8,70));
    EXPECT_CALL(_eepromInterfaceMock, update(9,1));
    EXPECT_CALL(_eepromInterfaceMock, update(10,14));
    EXPECT_CALL(_eepromInterfaceMock, update(11,0));
 
    _midiConfig.setChannel(15);
    _midiConfig.setOctave(6);
    _calibrationConfig.setMargin(12);
    _calibrationConfig.setVSeg(0, 1023);
    _calibrationConfig.setVSeg(1, 767);
    _calibrationConfig.setVSeg(2, 511);
    _calibrationConfig.setVSeg(3, 63);

    _memoryService.factoryReset();
}
