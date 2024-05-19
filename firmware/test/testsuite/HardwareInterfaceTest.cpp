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
    // All analog pins are pulled up
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

TEST_F(HardwareInterfaceTest, loop_check_calls)
{
    // All analog pins are read once
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A0));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A1));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A2));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A3));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A6));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A7));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A8));
    EXPECT_CALL(ArduinoMock::getInstance(), analogRead(A9));

    // Each pin is debounced
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).Times(NB_PIN*2);

    _hwInterface.loop();
}
