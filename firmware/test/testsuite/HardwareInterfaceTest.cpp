#include "gtest/gtest.h"

#include "ArduinoMock.h"
#include "MIDIConfig.h"
#include "CalibrationConfig.h"
#include "HardwareInterface.h"

class HardwareInterfaceTest : public ::testing::Test
{
public:
    HardwareInterfaceTest() : _hwInterface(_calibrationConfig) {};

    void SetUp() override { ArduinoMock::begin(); };
    void TearDown() override { ArduinoMock::end(); }

protected:
    MidiConfig _midiConfig;
    CalibrationConfig _calibrationConfig;
    HardwareInterface _hwInterface;
};

TEST_F(HardwareInterfaceTest, setup)
{
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(A0, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(A1, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(A2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(A3, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(A6, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(A7, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(A8, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(A9, INPUT_PULLUP));

    _hwInterface.setup();
}

