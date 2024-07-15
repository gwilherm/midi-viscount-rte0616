#include <gtest/gtest.h>
#include "ArduinoTestFixture.h"
#include "MIDIConfig.h"
#include "CalibrationConfig.h"
#include "HardwareInterface.h"
#include "MonodicNotesService.h"
#include "USBMIDIMock.h"
#include "SerialPrinter.h"

SerialPrinter Serial;

using namespace ::testing;

class FullChainTest : public ArduinoTestFixture {
public:
    FullChainTest():
        _calibrationConfig(50, 830, 512, 326, 14),
        _hwInterface(_calibrationConfig),
        _notesService(_usbMidiMock, _midiConfig, _hwInterface)
    {};

protected:
    StrictMock<USBMIDIMock> _usbMidiMock;
    MidiConfig _midiConfig;
    CalibrationConfig _calibrationConfig;
    HardwareInterface _hwInterface;
    MonodicNotesService _notesService;
};

std::unique_ptr<NiceMock<ArduinoMock>> ArduinoTestFixture::_arduinoMock;

TEST_F(FullChainTest, setup)
{
    _notesService.setup();
}

TEST_F(FullChainTest, note_c2_on_off)
{
    int millis = 0;

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(Return(millis));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(Return(830)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023));

    _hwInterface.loop();
    _notesService.loop();

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(Return(millis+=51));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(Return(830)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023));

    EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_ON, 36, 1));

    _hwInterface.loop();
    _notesService.loop();

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(Return(millis+=51));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023));

    _hwInterface.loop();
    _notesService.loop();

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(Return(millis+=51));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023));

    EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_OFF, 36, 1));

    _hwInterface.loop();
    _notesService.loop();
}

TEST_F(FullChainTest, note_f3_on_off)
{
    int millis = 0;

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(Return(millis));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(Return(1023)).WillOnce(Return(326)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023));

    _hwInterface.loop();
    _notesService.loop();

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(Return(millis+=51));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(Return(1023)).WillOnce(Return(326)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023));

    EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_ON, 53, 1));

    _hwInterface.loop();
    _notesService.loop();

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(Return(millis+=51));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023));

    _hwInterface.loop();
    _notesService.loop();

    EXPECT_CALL(*_arduinoMock, millis())
        .WillRepeatedly(Return(millis+=51));

    EXPECT_CALL(*_arduinoMock, analogRead)
        .WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023)).WillOnce(Return(1023));

    EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_OFF, 53, 1));

    _hwInterface.loop();
    _notesService.loop();
}
