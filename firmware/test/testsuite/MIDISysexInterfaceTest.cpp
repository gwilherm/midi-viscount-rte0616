#include "gtest/gtest.h"
#include <cstdint>

#include "ArduinoMock.h"
#include "SerialPrinter.h"
#include "MIDIConfig.h"
#include "CalibrationConfig.h"
#include "USBMIDIMock.h"
#include "MIDISysexInterface.h"
#include "ArduinoTestFixture.h"

SerialPrinter Serial;

#define EXPECT_SYSEX(e) [&](unsigned size, const uint8_t* array) { \
    EXPECT_EQ(size, sizeof(e)); \
    for (int i = 0; i < sizeof(e); i++) \
        EXPECT_EQ(array[i], e[i]); \
}

class MIDISysexInterfaceTest : public ::testing::Test {
public:
    MIDISysexInterfaceTest() : _sysexInterface(_usbMidiMock, _midiConfig, _calibrationConfig)
    {};

    void SetUp(void) {
        _midiConfig.reset();
        _calibrationConfig.reset();
    }

protected:
    MidiConfig _midiConfig;
    CalibrationConfig _calibrationConfig;
    ::testing::StrictMock<USBMIDIMock> _usbMidiMock;
    MIDISysexInterface _sysexInterface;
};

TEST_F(MIDISysexInterfaceTest, handle_ident_msg)
{
    // Ident request
    uint8_t req[] = { 0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7 };
    //                                 chan,                     f1,   f2,   p1,   p2,   v1,   v2,   v3,   v4
    const uint8_t exp_resp[] = { 0x7E, 0x01, 0x06, 0x02, 0x31, 0x00, 0x31, 0x06, 0x16, 0x01, 0x02, 0x00, 0x03 };

    using namespace ::testing;
    EXPECT_CALL(_usbMidiMock, sendSysEx(_, _))
    .WillOnce(Invoke(EXPECT_SYSEX(exp_resp)));

    _sysexInterface.handleSysEx(req, sizeof(req));
}

TEST_F(MIDISysexInterfaceTest, handle_get_midi_config)
{
    uint8_t req[] = { 0xF0, 0x31, 0x06, 0x16, 0x01, 0x01, 0xF7 };
                                                            // Default values
    const uint8_t exp_resp1[] = { 0x31, 0x06, 0x16, 0x01, 0x01, 1, 3 };

    using namespace ::testing;
    EXPECT_CALL(_usbMidiMock, sendSysEx(_, _))
    .WillOnce(Invoke(EXPECT_SYSEX(exp_resp1)));

    _sysexInterface.handleSysEx(req, sizeof(req));

    _midiConfig.setChannel(12);
    _midiConfig.setOctave(1);
    const uint8_t exp_resp2[] = { 0x31, 0x06, 0x16, 0x01, 0x01, 12, 1 };

    EXPECT_CALL(_usbMidiMock, sendSysEx(_, _))
    .WillOnce(Invoke(EXPECT_SYSEX(exp_resp2)));

    _sysexInterface.handleSysEx(req, sizeof(req));
}

TEST_F(MIDISysexInterfaceTest, handle_set_midi_config)
{
    uint8_t req[] = { 0xF0, 0x31, 0x06, 0x16, 0x01, 0x02, 12, 6, 0xF7 };
    const uint8_t exp_resp[] = { 0x31, 0x06, 0x16, 0x01, 0x01, 12, 6 };

    using namespace ::testing;
    EXPECT_CALL(_usbMidiMock, sendSysEx(_, _))
    .WillOnce(Invoke(EXPECT_SYSEX(exp_resp)));

    _sysexInterface.handleSysEx(req, sizeof(req));

    EXPECT_EQ(_midiConfig.getChannel(), 12);
    EXPECT_EQ(_midiConfig.getOctave(), 6);
}

TEST_F(MIDISysexInterfaceTest, handle_get_calibration_config)
{
    uint8_t req[] = { 0xF0, 0x31, 0x06, 0x16, 0x02, 0x01, 0xF7 };
                                                            // Default values
    const uint8_t exp_resp1[] = { 0x31, 0x06, 0x16, 0x02, 0x01, 0, 50, 6, 62, 4, 0, 2, 70, 0, 14 };
    // const uint8_t exp_resp1[] = { 0x31, 0x06, 0x16, 0x02, 0x01, 0, 50, 7, 37, 4, 113, 3, 58, 1, 2 };

    using namespace ::testing;
    EXPECT_CALL(_usbMidiMock, sendSysEx(_, _))
    .WillOnce(Invoke(EXPECT_SYSEX(exp_resp1)));

    _sysexInterface.handleSysEx(req, sizeof(req));

    _calibrationConfig.setMargin(12);
    _calibrationConfig.setVSeg(0, 1);
    _calibrationConfig.setVSeg(1, 2);
    _calibrationConfig.setVSeg(2, 3);
    _calibrationConfig.setVSeg(3, 4);

    const uint8_t exp_resp2[] = { 0x31, 0x06, 0x16, 0x02, 0x01, 0, 12, 0, 1, 0, 2, 0, 3, 0, 4 };

    EXPECT_CALL(_usbMidiMock, sendSysEx(_, _))
    .WillOnce(Invoke(EXPECT_SYSEX(exp_resp2)));

    _sysexInterface.handleSysEx(req, sizeof(req));
}

TEST_F(MIDISysexInterfaceTest, handle_set_calibration_config)
{
    uint8_t req[] = { 0xF0, 0x31, 0x06, 0x16, 0x02, 0x02, 0, 12, 0, 1, 0, 2, 0, 3, 0, 4, 0xF7 };
    const uint8_t exp_resp[] = { 0x31, 0x06, 0x16, 0x02, 0x01, 0, 12, 0, 1, 0, 2, 0, 3, 0, 4 };

    using namespace ::testing;
    EXPECT_CALL(_usbMidiMock, sendSysEx(_, _))
    .WillOnce(Invoke(EXPECT_SYSEX(exp_resp)));

    _sysexInterface.handleSysEx(req, sizeof(req));

    EXPECT_EQ(_calibrationConfig.getMargin(), 12);
    EXPECT_EQ(_calibrationConfig.getVSeg(0), 1);
    EXPECT_EQ(_calibrationConfig.getVSeg(1), 2);
    EXPECT_EQ(_calibrationConfig.getVSeg(2), 3);
    EXPECT_EQ(_calibrationConfig.getVSeg(3), 4);
}
