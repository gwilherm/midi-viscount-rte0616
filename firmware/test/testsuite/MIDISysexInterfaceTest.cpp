#include "gtest/gtest.h"

#include "ArduinoMock.h"
#include "SerialPrinter.h"
#include "MIDIConfig.h"
#include "CalibrationConfig.h"
#include "USBMIDIMock.h"
#include "MIDISysexInterface.h"
#include "ArduinoTestFixture.h"

SerialPrinter Serial;

class MIDISysexInterfaceTest : public ::testing::Test {
public:
    MIDISysexInterfaceTest() : _sysexInterface(_usbMidiMock, _midiConfig, _calibrationConfig)
    {};

protected:
    MidiConfig _midiConfig;
    CalibrationConfig _calibrationConfig;
    ::testing::StrictMock<USBMIDIMock> _usbMidiMock;
    MIDISysexInterface _sysexInterface;
};

TEST_F(MIDISysexInterfaceTest, handle_ident_msg)
{
    uint8_t data[] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7};
    _sysexInterface.handleSysEx(data, sizeof(data));
}