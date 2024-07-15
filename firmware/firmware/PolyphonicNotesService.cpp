#include "PolyphonicNotesService.h"

PolyphonicNotesService::PolyphonicNotesService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IHardwareInterface& hwInterface) :
    MIDINotesService(usbMidiInterface, midiConfig, hwInterface),
    _lastPinSegment{	PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG,
                        PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG }
{}

void PolyphonicNotesService::loop()
{
    pdlbrd_key_t newKey = PDLBRD_NO_KEY_PRESSED;
    int8_t* pinSegment = _hwInterface.getSegmentedValues();

    for (uint8_t pin = 0; pin < NB_PIN; pin++)
    {
        pdlbrd_key_t k = PDLBRD_KEY_MAP[pinSegment[pin]][pin];
        if (pinSegment[pin] != _lastPinSegment[pin])
        {
            if (_lastPinSegment[pin] != PDLBRD_NO_SEG)
                sendNote(IMIDIInterface::NOTE_OFF, PDLBRD_KEY_MAP[_lastPinSegment[pin]][pin]);

            if (pinSegment[pin] != PDLBRD_NO_SEG)
                sendNote(IMIDIInterface::NOTE_ON, k);

            _lastPinSegment[pin] = pinSegment[pin];
        }
    }
}