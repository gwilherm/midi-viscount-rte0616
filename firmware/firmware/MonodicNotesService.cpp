#include "MonodicNotesService.h"
#include "pdlbrdkeys.h"

MonodicNotesService::MonodicNotesService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IHardwareInterface& hwInterface) :
    _usbMidiInterface(usbMidiInterface),
    _midiConfig(midiConfig),
    _hwInterface(hwInterface),
    _currentKey(PDLBRD_NO_KEY_PRESSED)
{}

void MonodicNotesService::reset()
{
    _currentKey = PDLBRD_NO_KEY_PRESSED;
}

void MonodicNotesService::loop()
{
    pdlbrd_key_t newKey = PDLBRD_NO_KEY_PRESSED;
    int8_t* pinSegment = _hwInterface.getSegmentedValues();

    for (uint8_t pin = 0; pin < NB_PIN; pin++)
    {
        if (pinSegment[pin] != PDLBRD_NO_SEG)
        {
            pdlbrd_key_t k = PDLBRD_KEY_MAP[pinSegment[pin]][pin];
            if ((newKey == PDLBRD_NO_KEY_PRESSED) || (k != PDLBRD_NO_KEY_PRESSED) && (k > newKey))
                newKey = k;
        }
    }
#if defined (FW_DEBUG_VERBOSE_RTE0616PEDALBOARD)
    Serial.println(PDLBRD_KEY_STR[newKey]);
#endif

    if (_currentKey != newKey) {

        if (_currentKey != PDLBRD_NO_KEY_PRESSED) {
            sendNote(_usbMidiInterface, _midiConfig, IMIDIInterface::NOTE_OFF, _currentKey);
        }

        if (newKey != PDLBRD_NO_KEY_PRESSED) {
            sendNote(_usbMidiInterface, _midiConfig, IMIDIInterface::NOTE_ON, newKey);
        }

        _currentKey = newKey;
    }
}