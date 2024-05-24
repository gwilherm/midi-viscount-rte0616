#include "MIDINotesService.h"

#define TONES_IN_OCTAVE 12

MIDINotesService::MIDINotesService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IHardwareInterface& hwInterface) :
    _usbMidiInterface(usbMidiInterface),
	_midiConfig(midiConfig),
    _hwInterface(hwInterface),
    _currentKey(PDLBRD_NO_KEY_PRESSED)
{}

void MIDINotesService::loop()
{
    pdlbrd_key_t newKey = PDLBRD_NO_KEY_PRESSED;
    int8_t* pinSegment = _hwInterface.getSegmentedValues();

	for (uint8_t seg = 0; seg < PDLBRD_NB_SEG; seg++)
	{
		for (uint8_t pin = 0; pin < NB_PIN; pin++)
		{
			if (pinSegment[pin] != PDLBRD_NO_SEG)
			{
				pdlbrd_key_t k = PDLBRD_KEY_MAP[pinSegment[pin]][pin];
				if ((newKey == PDLBRD_NO_KEY_PRESSED) || (k != PDLBRD_NO_KEY_PRESSED) && (k > newKey))
					newKey = k;
			}
		}
	}
#if defined (FW_DEBUG_VERBOSE_RTE0616PEDALBOARD)
    Serial.println(PDLBRD_KEY_STR[newKey]);
#endif

	if (_currentKey != newKey) {
		if (_currentKey != PDLBRD_NO_KEY_PRESSED)
			_usbMidiInterface.sendNoteOff((_midiConfig.getOctave()*TONES_IN_OCTAVE)+_currentKey, _midiConfig.getChannel());
		if (newKey != PDLBRD_NO_KEY_PRESSED)
			_usbMidiInterface.sendNoteOn((_midiConfig.getOctave()*TONES_IN_OCTAVE)+newKey, _midiConfig.getChannel());
		_currentKey = newKey;
	}
}