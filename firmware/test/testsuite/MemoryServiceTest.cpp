#include <gtest/gtest.h>
#include "MIDIConfig.h"
#include "EEPROMInterfaceMock.h"
#include "MemoryService.h"
#include "SerialPrinter.h"

SerialPrinter Serial;

using namespace ::testing;

class MemoryServiceTest : public Test {
public:
    MemoryServiceTest():
        _midiConfig(1, 3, MidiConfig::MONODIC_UP),
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
    /* Device Version */
    EXPECT_CALL(_eepromInterfaceMock, update( 0, 0x31));
    EXPECT_CALL(_eepromInterfaceMock, update( 1, 0x06));
    EXPECT_CALL(_eepromInterfaceMock, update( 2, 0x16));
    EXPECT_CALL(_eepromInterfaceMock, update( 3, 1)); // v1.
    EXPECT_CALL(_eepromInterfaceMock, update( 4, 2)); //  2.
    EXPECT_CALL(_eepromInterfaceMock, update( 5, 0));
    EXPECT_CALL(_eepromInterfaceMock, update( 6, 3)); //  3

    /* Calibration config */
    EXPECT_CALL(_eepromInterfaceMock, update( 7, 12));
    EXPECT_CALL(_eepromInterfaceMock, update( 8, 0));
    EXPECT_CALL(_eepromInterfaceMock, update( 9, 255));
    EXPECT_CALL(_eepromInterfaceMock, update(10, 3));
    EXPECT_CALL(_eepromInterfaceMock, update(11, 255));
    EXPECT_CALL(_eepromInterfaceMock, update(12, 2));
    EXPECT_CALL(_eepromInterfaceMock, update(13, 255));
    EXPECT_CALL(_eepromInterfaceMock, update(14, 1));
    EXPECT_CALL(_eepromInterfaceMock, update(15, 63));
    EXPECT_CALL(_eepromInterfaceMock, update(16, 0));

    /* MIDI Config */
    EXPECT_CALL(_eepromInterfaceMock, update(17, 1));
    EXPECT_CALL(_eepromInterfaceMock, update(18, 3));
    EXPECT_CALL(_eepromInterfaceMock, update(19, 0));

    _memoryService.store();

    /* Device Version */
    EXPECT_CALL(_eepromInterfaceMock, update( 0, 0x31));
    EXPECT_CALL(_eepromInterfaceMock, update( 1, 0x06));
    EXPECT_CALL(_eepromInterfaceMock, update( 2, 0x16));
    EXPECT_CALL(_eepromInterfaceMock, update( 3, 1)); // v1.
    EXPECT_CALL(_eepromInterfaceMock, update( 4, 2)); //  2.
    EXPECT_CALL(_eepromInterfaceMock, update( 5, 0));
    EXPECT_CALL(_eepromInterfaceMock, update( 6, 3)); //  3


    /* Calibration config */
    EXPECT_CALL(_eepromInterfaceMock, update( 7, 12));
    EXPECT_CALL(_eepromInterfaceMock, update( 8, 0));
    EXPECT_CALL(_eepromInterfaceMock, update( 9, 255));
    EXPECT_CALL(_eepromInterfaceMock, update(10, 3));
    EXPECT_CALL(_eepromInterfaceMock, update(11, 255));
    EXPECT_CALL(_eepromInterfaceMock, update(12, 2));
    EXPECT_CALL(_eepromInterfaceMock, update(13, 255));
    EXPECT_CALL(_eepromInterfaceMock, update(14, 1));
    EXPECT_CALL(_eepromInterfaceMock, update(15, 63));
    EXPECT_CALL(_eepromInterfaceMock, update(16, 0));

    /* MIDI Config */
    EXPECT_CALL(_eepromInterfaceMock, update(17, 15));
    EXPECT_CALL(_eepromInterfaceMock, update(18, 6));
    EXPECT_CALL(_eepromInterfaceMock, update(19, 0));

    _midiConfig.setChannel(15);
    _midiConfig.setOctave(6);
    _midiConfig.setKeyboardMode(MidiConfig::MONODIC_UP);

    _memoryService.store();
}

TEST_F(MemoryServiceTest, restore)
{
    /* Check device mem + read Device Version */
    EXPECT_CALL(_eepromInterfaceMock, read(0)).Times(2)
        .WillRepeatedly(Return(0x31));
    EXPECT_CALL(_eepromInterfaceMock, read(1)).Times(2)
        .WillRepeatedly(Return(0x06));
    EXPECT_CALL(_eepromInterfaceMock, read(2)).Times(2)
        .WillRepeatedly(Return(0x16));
    EXPECT_CALL(_eepromInterfaceMock, read(3)).Times(2)
        .WillRepeatedly(Return(0x01));
    EXPECT_CALL(_eepromInterfaceMock, read(4))
        .WillOnce(Return(0x02));
    EXPECT_CALL(_eepromInterfaceMock, read(5))
        .WillOnce(Return(0x00));
    EXPECT_CALL(_eepromInterfaceMock, read(6))
        .WillOnce(Return(0x03));

    /* Calibration Config */
    EXPECT_CALL(_eepromInterfaceMock, read(7))
        .WillOnce(Return(12));
    EXPECT_CALL(_eepromInterfaceMock, read(8))
        .WillOnce(Return(0));
    EXPECT_CALL(_eepromInterfaceMock, read(9))
        .WillOnce(Return(255));
    EXPECT_CALL(_eepromInterfaceMock, read(10))
        .WillOnce(Return(3));
    EXPECT_CALL(_eepromInterfaceMock, read(11))
        .WillOnce(Return(255));
    EXPECT_CALL(_eepromInterfaceMock, read(12))
        .WillOnce(Return(2));
    EXPECT_CALL(_eepromInterfaceMock, read(13))
        .WillOnce(Return(255));
    EXPECT_CALL(_eepromInterfaceMock, read(14))
        .WillOnce(Return(1));
    EXPECT_CALL(_eepromInterfaceMock, read(15))
        .WillOnce(Return(63));
    EXPECT_CALL(_eepromInterfaceMock, read(16))
        .WillOnce(Return(0));

    /* MIDI Config */
    EXPECT_CALL(_eepromInterfaceMock, read(17))
        .WillOnce(Return(15));
    EXPECT_CALL(_eepromInterfaceMock, read(18))
        .WillOnce(Return(6));
    EXPECT_CALL(_eepromInterfaceMock, read(19))
        .WillOnce(Return(0));

    _memoryService.restore();

    EXPECT_EQ(_midiConfig.getChannel(), 15);
    EXPECT_EQ(_midiConfig.getOctave(), 6);
    EXPECT_EQ(_midiConfig.getKeyboardMode(), MidiConfig::MONODIC_UP);
    EXPECT_EQ(_calibrationConfig.getMargin(), 12);
    EXPECT_EQ(_calibrationConfig.getVSeg(0), 1023);
    EXPECT_EQ(_calibrationConfig.getVSeg(1), 767);
    EXPECT_EQ(_calibrationConfig.getVSeg(2), 511);
    EXPECT_EQ(_calibrationConfig.getVSeg(3), 63);
}

#define EXPECT_FACTORY_RESET_CALLS() \
    /* Device Version */ \
    EXPECT_CALL(_eepromInterfaceMock, update( 0,0x31)); \
    EXPECT_CALL(_eepromInterfaceMock, update( 1,0x06)); \
    EXPECT_CALL(_eepromInterfaceMock, update( 2,0x16)); \
    EXPECT_CALL(_eepromInterfaceMock, update( 3,1)); \
    EXPECT_CALL(_eepromInterfaceMock, update( 4,2)); \
    EXPECT_CALL(_eepromInterfaceMock, update( 5,0)); \
    EXPECT_CALL(_eepromInterfaceMock, update( 6,3)); \
 \
    /* Calibration Config */ \
    EXPECT_CALL(_eepromInterfaceMock, update( 7,50)); \
    EXPECT_CALL(_eepromInterfaceMock, update( 8,0)); \
    EXPECT_CALL(_eepromInterfaceMock, update( 9,62)); \
    EXPECT_CALL(_eepromInterfaceMock, update(10,3)); \
    EXPECT_CALL(_eepromInterfaceMock, update(11,0)); \
    EXPECT_CALL(_eepromInterfaceMock, update(12,2)); \
    EXPECT_CALL(_eepromInterfaceMock, update(13,70)); \
    EXPECT_CALL(_eepromInterfaceMock, update(14,1)); \
    EXPECT_CALL(_eepromInterfaceMock, update(15,14)); \
    EXPECT_CALL(_eepromInterfaceMock, update(16,0)); \
 \
    /* MIDI Config */ \
    EXPECT_CALL(_eepromInterfaceMock, update(17,1)); \
    EXPECT_CALL(_eepromInterfaceMock, update(18,3)); \
    EXPECT_CALL(_eepromInterfaceMock, update(19,0)); \

TEST_F(MemoryServiceTest, factory_reset)
{
    EXPECT_FACTORY_RESET_CALLS();

    _midiConfig.setChannel(15);
    _midiConfig.setOctave(6);
    _midiConfig.setKeyboardMode(MidiConfig::MONODIC_UP);
    _calibrationConfig.setMargin(12);
    _calibrationConfig.setVSeg(0, 1023);
    _calibrationConfig.setVSeg(1, 767);
    _calibrationConfig.setVSeg(2, 511);
    _calibrationConfig.setVSeg(3, 63);

    _memoryService.factoryReset();
}

TEST_F(MemoryServiceTest, restore_unexpected_data)
{
    /* Device Version */
    EXPECT_CALL(_eepromInterfaceMock, read(0))
        .WillOnce(Return('B'));
    EXPECT_CALL(_eepromInterfaceMock, read(1))
        .WillOnce(Return('A'));
    EXPECT_CALL(_eepromInterfaceMock, read(2))
        .WillOnce(Return('A'));
    EXPECT_CALL(_eepromInterfaceMock, read(3))
        .WillOnce(Return('D'));

    EXPECT_FACTORY_RESET_CALLS();

    _memoryService.restore();
}

TEST_F(MemoryServiceTest, restore_old_version)
{
    /* Device Version */
    EXPECT_CALL(_eepromInterfaceMock, read(0))
        .WillOnce(Return(0x31));
    EXPECT_CALL(_eepromInterfaceMock, read(1))
        .WillOnce(Return(0x06));
    EXPECT_CALL(_eepromInterfaceMock, read(2))
        .WillOnce(Return(0x16));
    EXPECT_CALL(_eepromInterfaceMock, read(3))
        .WillOnce(Return(0));

    EXPECT_FACTORY_RESET_CALLS();

    _memoryService.restore();
}