#include "gtest/gtest.h"

#include "ArduinoMock.h"
#include "MIDIConfig.h"
#include "CalibrationConfig.h"
#include "HardwareInterface.h"
#include "ArduinoTestFixture.h"

class HardwareInterfaceTest : public ArduinoTestFixture
{
public:
    HardwareInterfaceTest() : _hwInterface(_calibrationConfig) {};

protected:
    MidiConfig _midiConfig;
    CalibrationConfig _calibrationConfig;
    HardwareInterface _hwInterface;
};

std::unique_ptr<::testing::NiceMock<ArduinoMock>> ArduinoTestFixture::_arduinoMock;

TEST_F(HardwareInterfaceTest, setup)
{
    // All analog pins are pulled up
    EXPECT_CALL(*_arduinoMock, pinMode(A0, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(A1, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(A2, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(A3, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(A6, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(A7, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(A8, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(A9, INPUT_PULLUP));

    _hwInterface.setup();
}

TEST_F(HardwareInterfaceTest, loop_check_calls)
{
    // All analog pins are read once
    EXPECT_CALL(*_arduinoMock, analogRead(A0));
    EXPECT_CALL(*_arduinoMock, analogRead(A1));
    EXPECT_CALL(*_arduinoMock, analogRead(A2));
    EXPECT_CALL(*_arduinoMock, analogRead(A3));
    EXPECT_CALL(*_arduinoMock, analogRead(A6));
    EXPECT_CALL(*_arduinoMock, analogRead(A7));
    EXPECT_CALL(*_arduinoMock, analogRead(A8));
    EXPECT_CALL(*_arduinoMock, analogRead(A9));

    // Each pin is debounced
    EXPECT_CALL(*_arduinoMock, millis()).Times(NB_PIN*2);

    _hwInterface.loop();
}

TEST_F(HardwareInterfaceTest, loop_raw_values)
{
    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(::testing::Return(127))
        .WillOnce(::testing::Return(255))
        .WillOnce(::testing::Return(383))
        .WillOnce(::testing::Return(511))
        .WillOnce(::testing::Return(639))
        .WillOnce(::testing::Return(767))
        .WillOnce(::testing::Return(895))
        .WillOnce(::testing::Return(1023));

    _hwInterface.loop();

    int* rawValues = _hwInterface.getRawValues();
    EXPECT_EQ(rawValues[0], 127);
    EXPECT_EQ(rawValues[1], 255);
    EXPECT_EQ(rawValues[2], 383);
    EXPECT_EQ(rawValues[3], 511);
    EXPECT_EQ(rawValues[4], 639);
    EXPECT_EQ(rawValues[5], 767);
    EXPECT_EQ(rawValues[6], 895);
    EXPECT_EQ(rawValues[7], 1023);
}

TEST_F(HardwareInterfaceTest, loop_segmented_values_first_loop)
{
    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(::testing::Return(15))
        .WillOnce(::testing::Return(255))
        .WillOnce(::testing::Return(326))
        .WillOnce(::testing::Return(511))
        .WillOnce(::testing::Return(639))
        .WillOnce(::testing::Return(767))
        .WillOnce(::testing::Return(850))
        .WillOnce(::testing::Return(1023));

    _hwInterface.loop();

    int8_t* segmentedValues = _hwInterface.getSegmentedValues();

    // Debounced
    EXPECT_EQ(segmentedValues[0], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[1], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[2], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[3], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[4], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[5], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[6], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[7], PDLBRD_NO_SEG);
}

TEST_F(HardwareInterfaceTest, loop_segmented_values_50ms)
{
    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(::testing::Return(0));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(::testing::Return(15))
        .WillOnce(::testing::Return(255))
        .WillOnce(::testing::Return(326))
        .WillOnce(::testing::Return(511))
        .WillOnce(::testing::Return(639))
        .WillOnce(::testing::Return(767))
        .WillOnce(::testing::Return(850))
        .WillOnce(::testing::Return(1023));

    _hwInterface.loop();

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(::testing::Return(50));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(::testing::Return(15))
        .WillOnce(::testing::Return(255))
        .WillOnce(::testing::Return(326))
        .WillOnce(::testing::Return(511))
        .WillOnce(::testing::Return(639))
        .WillOnce(::testing::Return(767))
        .WillOnce(::testing::Return(850))
        .WillOnce(::testing::Return(1023));

    _hwInterface.loop();

    int8_t* segmentedValues = _hwInterface.getSegmentedValues();

    // Debounced
    EXPECT_EQ(segmentedValues[0], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[1], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[2], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[3], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[4], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[5], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[6], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[7], PDLBRD_NO_SEG);
}

TEST_F(HardwareInterfaceTest, loop_segmented_values_51ms)
{
    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(::testing::Return(0));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(::testing::Return(15))
        .WillOnce(::testing::Return(255))
        .WillOnce(::testing::Return(326))
        .WillOnce(::testing::Return(511))
        .WillOnce(::testing::Return(639))
        .WillOnce(::testing::Return(767))
        .WillOnce(::testing::Return(850))
        .WillOnce(::testing::Return(1023));

    _hwInterface.loop();

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(::testing::Return(51));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(::testing::Return(15))
        .WillOnce(::testing::Return(255))
        .WillOnce(::testing::Return(326))
        .WillOnce(::testing::Return(511))
        .WillOnce(::testing::Return(639))
        .WillOnce(::testing::Return(767))
        .WillOnce(::testing::Return(850))
        .WillOnce(::testing::Return(1023));

    _hwInterface.loop();

    int8_t* segmentedValues = _hwInterface.getSegmentedValues();
    EXPECT_EQ(segmentedValues[0], PDLBRD_SEG_4);
    EXPECT_EQ(segmentedValues[1], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[2], PDLBRD_SEG_3);
    EXPECT_EQ(segmentedValues[3], PDLBRD_SEG_2);
    EXPECT_EQ(segmentedValues[4], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[5], PDLBRD_NO_SEG);
    EXPECT_EQ(segmentedValues[6], PDLBRD_SEG_1);
    EXPECT_EQ(segmentedValues[7], PDLBRD_NO_SEG);
}
