#include <gtest/gtest.h>
#include "MIDIConfig.h"
#include "CalibrationConfig.h"
#include "HardwareInterfaceMock.h"
#include "MonodicNotesService.h"
#include "USBMIDIMock.h"
#include "SerialPrinter.h"
#include <cstdint>

SerialPrinter Serial;

using namespace ::testing;

class MonodicNotesServiceTest : public Test {
public:
    MonodicNotesServiceTest():
        _midiConfig(1, 3, MidiConfig::MONODIC_UP),
        _notesService(_usbMidiMock, _midiConfig, _hwInterfaceMock)
    {};

    void SetUp() { for (int i = 0; i < NB_PIN; i++) _segmentedValues[i] = PDLBRD_NO_SEG; };

protected:
    StrictMock<USBMIDIMock> _usbMidiMock;
    MidiConfig _midiConfig;
    CalibrationConfig _calibrationConfig;
    HardwareInterfaceMock _hwInterfaceMock;
    MonodicNotesService _notesService;

    int8_t _segmentedValues[NB_PIN];
};

// Press and release every pedal from C2 to D4
TEST_F(MonodicNotesServiceTest, monodic_note_each_on_off)
{
    int expected_note = 36; // C2
    for (int seg = 0; seg < PDLBRD_NB_SEG; seg++)
    {
        for (int pin = 0; pin < NB_PIN; pin++)
        {
            _segmentedValues[pin] = seg;
            EXPECT_CALL(_hwInterfaceMock, getSegmentedValues)
            .WillOnce(Return(_segmentedValues));

            EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_ON, expected_note, 1));
            
            _notesService.loop();

            _segmentedValues[pin] = PDLBRD_NO_SEG;
            EXPECT_CALL(_hwInterfaceMock, getSegmentedValues)
            .WillOnce(Return(_segmentedValues));

            EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_OFF, expected_note, 1));

            _notesService.loop();

            if(++expected_note > 62) break; // D4
        }
    }
}

// Press every pedal from C2 to D4 but do not release any
TEST_F(MonodicNotesServiceTest, monodic_up_prio)
{
    int expected_note = 36; // C2
    for (int seg = 0; seg < PDLBRD_NB_SEG; seg++)
    {
        for (int pin = 0; pin < NB_PIN; pin++)
        {
            if (expected_note > 36) // Nothing to set to off for the first pedal
            {
                // Previous note is automatically set to off when pressing an upper pedal
                EXPECT_CALL(_hwInterfaceMock, getSegmentedValues)
                .WillOnce(Return(_segmentedValues));

                EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_OFF, expected_note-1, 1));

                _notesService.loop();
            }

            _segmentedValues[pin] = seg;
            EXPECT_CALL(_hwInterfaceMock, getSegmentedValues)
            .WillOnce(Return(_segmentedValues));

            EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_ON, expected_note, 1));
            
            _notesService.loop();

            if(++expected_note > 62) break; // D4
        }
    }
}

// Press D4, nothing happens if we press any lower key
TEST_F(MonodicNotesServiceTest, monodic_down_not_prio)
{
    _segmentedValues[2] = PDLBRD_SEG_4; //D4
    EXPECT_CALL(_hwInterfaceMock, getSegmentedValues)
    .WillOnce(Return(_segmentedValues));

    EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_ON, 62, 1));
    
    _notesService.loop();

    for (int seg = 0; seg < PDLBRD_NB_SEG; seg++)
    {
        for (int pin = 0; pin < NB_PIN; pin++)
        {
            // Physically impossible to change pin2 value, it is already on the highest raw value
            if (pin != 2) {
                _segmentedValues[pin] = seg;
                EXPECT_CALL(_hwInterfaceMock, getSegmentedValues)
                .WillOnce(Return(_segmentedValues));
                
                _notesService.loop();
            }
        }
    }
}

TEST_F(MonodicNotesServiceTest, note_octave_6)
{
    _midiConfig.setOctave(6);

    _segmentedValues[0] = PDLBRD_SEG_1;
    EXPECT_CALL(_hwInterfaceMock, getSegmentedValues)
    .WillOnce(Return(_segmentedValues));

    EXPECT_CALL(_usbMidiMock, sendNote(IMIDIInterface::NOTE_ON, 72, 1));
    
    _notesService.loop();
}
