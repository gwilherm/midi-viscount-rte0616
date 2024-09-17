#include "gtest/gtest.h"

#include "ArduinoMock.h"
#include "SerialPrinter.h"
#include "MIDIConfig.h"
#include "CalibrationConfig.h"
#include "ExtraPedalInterface.h"
#include "ArduinoTestFixture.h"

SerialPrinter Serial;

class ExtraPedalInterfaceTest : public ArduinoTestFixture
{
public:
    ExtraPedalInterfaceTest() {};

protected:
    ExtraPedalInterface _extraPedalInterface;
};

std::unique_ptr<::testing::NiceMock<ArduinoMock>> ArduinoTestFixture::_arduinoMock;

TEST_F(ExtraPedalInterfaceTest, setup)
{
    // // All analog pins are pulled up
    EXPECT_CALL(*_arduinoMock, pinMode(A10, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(3, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(14, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(15, INPUT_PULLUP));
    EXPECT_CALL(*_arduinoMock, pinMode(16, INPUT_PULLUP));

    _extraPedalInterface.setup();
}

TEST_F(ExtraPedalInterfaceTest, loop_check_calls)
{
    // All analog pins are read once
    EXPECT_CALL(*_arduinoMock, analogRead(A10));
    EXPECT_CALL(*_arduinoMock, digitalRead(2));
    EXPECT_CALL(*_arduinoMock, digitalRead(3));
    EXPECT_CALL(*_arduinoMock, digitalRead(14));
    EXPECT_CALL(*_arduinoMock, digitalRead(15));
    EXPECT_CALL(*_arduinoMock, digitalRead(16));

    // Debounced
    EXPECT_CALL(*_arduinoMock, millis()).WillRepeatedly(::testing::Return(0));

    _extraPedalInterface.loop();
}

TEST_F(ExtraPedalInterfaceTest, loop_get_expression_value)
{
    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(::testing::Return(0))
        .WillOnce(::testing::Return(1023));

    _extraPedalInterface.loop();
    uint32_t exprValue = _extraPedalInterface.getExpressionValue();
    EXPECT_EQ(exprValue, 0);

    _extraPedalInterface.loop();
    exprValue = _extraPedalInterface.getExpressionValue();
    EXPECT_EQ(exprValue, 1023);
}

TEST_F(ExtraPedalInterfaceTest, loop_get_switch_value_50ms)
{
    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(::testing::Return(0));
    EXPECT_CALL(*_arduinoMock, digitalRead)
        .WillRepeatedly(::testing::Return(HIGH));

    _extraPedalInterface.loop();

    for (int i = 0; i < NB_SWITCH; i++)
    {
        bool swState = _extraPedalInterface.getSwitchState(i);
        EXPECT_EQ(swState, false);
    }


    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(::testing::Return(50));
    EXPECT_CALL(*_arduinoMock, digitalRead)
    .WillRepeatedly(::testing::Return(LOW));

    _extraPedalInterface.loop();

    for (int i = 0; i < NB_SWITCH; i++)
    {
        bool swState = _extraPedalInterface.getSwitchState(i);
        EXPECT_EQ(swState, false);
    }
}

TEST_F(ExtraPedalInterfaceTest, loop_get_switch_value_51ms)
{
    int curTimeMs = 0;

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(::testing::Return(curTimeMs));
    EXPECT_CALL(*_arduinoMock, digitalRead)
        .WillRepeatedly(::testing::Return(HIGH));

    _extraPedalInterface.loop();

    for (int i = 0; i < NB_SWITCH; i++)
    {
        bool swState = _extraPedalInterface.getSwitchState(i);
        EXPECT_EQ(swState, false);
    }

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(::testing::Return(curTimeMs+=50));
    EXPECT_CALL(*_arduinoMock, digitalRead)
    .WillRepeatedly(::testing::Return(LOW));

    _extraPedalInterface.loop();

    for (int i = 0; i < NB_SWITCH; i++)
    {
        bool swState = _extraPedalInterface.getSwitchState(i);
        EXPECT_EQ(swState, false);
    }

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(::testing::Return(curTimeMs+=51));
    EXPECT_CALL(*_arduinoMock, digitalRead)
    .WillRepeatedly(::testing::Return(LOW));

    _extraPedalInterface.loop();

    for (int i = 0; i < NB_SWITCH; i++)
    {
        bool swState = _extraPedalInterface.getSwitchState(i);
        EXPECT_EQ(swState, true);
    }
}