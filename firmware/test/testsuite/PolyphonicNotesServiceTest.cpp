#include <gtest/gtest.h>
#include "MIDIConfig.h"
#include "CalibrationConfig.h"
#include "HardwareInterfaceMock.h"
#include "PolyphonicNotesService.h"
#include "USBMIDIMock.h"
#include "SerialPrinter.h"
#include <cstdint>

SerialPrinter Serial;

using namespace ::testing;

class PolyphonicNotesServiceTest : public Test {
public:
    PolyphonicNotesServiceTest():
        _midiConfig(1, 3),
        _notesService(_usbMidiMock, _midiConfig, _hwInterfaceMock)
    {};

    void SetUp() { for (int i = 0; i < NB_PIN; i++) _segmentedValues[i] = PDLBRD_NO_SEG; };

protected:
    StrictMock<USBMIDIMock> _usbMidiMock;
    MidiConfig _midiConfig;
    CalibrationConfig _calibrationConfig;
    HardwareInterfaceMock _hwInterfaceMock;
    PolyphonicNotesService _notesService;

    int8_t _segmentedValues[NB_PIN];
};

TEST_F(PolyphonicNotesServiceTest, setup)
{
    _notesService.setup();
}

// Press every pedal from C2 to D4
TEST_F(PolyphonicNotesServiceTest, poly_each_on)
{
    _midiConfig.setKeyboardMode(MidiConfig::POLYPHONIC);

    int expected_note = 36; // C2
    for (int seg = 0; seg < PDLBRD_NB_SEG; seg++)
    {
        for (int pin = 0; pin < NB_PIN; pin++)
        {
            _segmentedValues[pin] = seg;
            EXPECT_CALL(_hwInterfaceMock, getSegmentedValues)
            .WillOnce(Return(_segmentedValues));

            EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_ON, expected_note, 1));

            // Note automatically turns off because a note
            // and its augmented fifth are using the same pin
            if (seg > PDLBRD_SEG_1)
                EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_OFF, expected_note - NB_PIN, 1));

            _notesService.loop();

            if(++expected_note >= 64) break; // D4
        }
    }
}

// Press C2 + D2, then G#2
TEST_F(PolyphonicNotesServiceTest, poly_c2d2gs2)
{
    _segmentedValues[0] = PDLBRD_SEG_1; // C2
    _segmentedValues[2] = PDLBRD_SEG_1; // D2
    EXPECT_CALL(_hwInterfaceMock, getSegmentedValues)
        .WillOnce(Return(_segmentedValues));

    EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_ON, 36, 1));
    EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_ON, 38, 1));

    _notesService.loop();

    _segmentedValues[0] = PDLBRD_SEG_2; // G#2
    EXPECT_CALL(_hwInterfaceMock, getSegmentedValues)
        .WillOnce(Return(_segmentedValues));

    // C2 automatically turns off because a note
    // and its augmented fifth are using the same pin
    EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_OFF, 36, 1));
    EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_ON, 44, 1));
    
    _notesService.loop();
}
