#include <gtest/gtest.h>
#include "MIDIConfig.h"
#include "CalibrationConfig.h"
#include "PedalInterfaceMock.h"
#include "MIDICCService.h"
#include "USBMIDIMock.h"
#include "SerialPrinter.h"
#include <cstdint>

SerialPrinter Serial;

using namespace ::testing;

class MIDICCServiceTest : public Test {
public:
    MIDICCServiceTest():
        _midiConfig(1, 3, MidiConfig::MONODIC_UP),
        _ccService(_usbMidiMock, _midiConfig, _pedalInterfaceMock)
    {};

protected:
    StrictMock<USBMIDIMock> _usbMidiMock;
    MidiConfig _midiConfig;
    PedalInterfaceMock _pedalInterfaceMock;
    MIDICCService _ccService;
};

TEST_F(MIDICCServiceTest, setup)
{
    _ccService.setup();
}

TEST_F(MIDICCServiceTest, loop)
{
    // Half expression
    EXPECT_CALL(_pedalInterfaceMock, getExpressionValue)
    .WillOnce(Return(512));
    EXPECT_CALL(_pedalInterfaceMock, getSwitchState)
    .WillRepeatedly(Return(true));

    
    EXPECT_CALL(_usbMidiMock, sendCC(11, 64, 1));
    EXPECT_CALL(_usbMidiMock, sendCC(64, 127, 1));

    _ccService.loop();
}

