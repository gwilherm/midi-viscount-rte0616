#include <gtest/gtest.h>
#include "MIDIConfig.h"
#include "MIDINotesServiceManager.h"
#include "USBMIDIMock.h"
#include "MIDINotesServiceMock.h"
#include "SerialPrinter.h"

SerialPrinter Serial;

using namespace ::testing;

class NotesServiceManagerTest : public Test {
public:
    NotesServiceManagerTest():
        _midiConfig(1, 3, MidiConfig::MONODIC_UP),
        _notesServiceManager(_midiConfig, _usbMidiMock, _notesServiceArray[MidiConfig::MONODIC_UP], _notesServiceArray[MidiConfig::POLYPHONIC])
    {};

    void SetUp() { };

protected:
    MidiConfig _midiConfig;
    StrictMock<USBMIDIMock> _usbMidiMock;
    MIDINotesServiceMock _notesServiceArray[MidiConfig::KEYBOARD_MODE_MAX+1]; 
    MIDINotesServiceManager _notesServiceManager;
};

TEST_F(NotesServiceManagerTest, setup)
{
    EXPECT_CALL(_notesServiceArray[MidiConfig::MONODIC_UP], setup());
    EXPECT_CALL(_notesServiceArray[MidiConfig::POLYPHONIC], setup());

    _notesServiceManager.setup();
}

TEST_F(NotesServiceManagerTest, loop)
{
    EXPECT_CALL(_notesServiceArray[MidiConfig::MONODIC_UP], loop());

    _notesServiceManager.loop();
}


TEST_F(NotesServiceManagerTest, change_mode)
{
    EXPECT_CALL(_notesServiceArray[MidiConfig::MONODIC_UP], loop());

    _notesServiceManager.loop();

    _midiConfig.setKeyboardMode(MidiConfig::POLYPHONIC);

    EXPECT_CALL(_usbMidiMock, sendCC(123, 0, 1));
    EXPECT_CALL(_notesServiceArray[MidiConfig::MONODIC_UP], reset());
    EXPECT_CALL(_notesServiceArray[MidiConfig::POLYPHONIC], loop());

    _notesServiceManager.loop();
}
