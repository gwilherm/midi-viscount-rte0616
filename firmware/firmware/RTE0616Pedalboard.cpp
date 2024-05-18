#include "RTE0616Pedalboard.h"

#include <Arduino.h>
#include "CalibrationConfig.h"
#include "HardwareInterface.h"
#include "MIDIConfig.h"
#include "MIDISysexInterface.h"
#include "pdlbrdkeys.h"


#include <USB-MIDI.h>
USBMIDI_CREATE_DEFAULT_INSTANCE();

#define TONES_IN_OCTAVE 12

uint8_t velocity = 127;

pdlbrd_key_t currentKey = PDLBRD_NO_KEY_PRESSED;


MidiConfig RTE0616Pedalboard::_midiConfig;
CalibrationConfig RTE0616Pedalboard::_calibrationConfig;
HardwareInterface RTE0616Pedalboard::_hwInterface(RTE0616Pedalboard::_calibrationConfig);
MIDISysexInterface RTE0616Pedalboard::_sysexInterface(MIDI, RTE0616Pedalboard::_midiConfig, RTE0616Pedalboard::_calibrationConfig);

RTE0616Pedalboard::RTE0616Pedalboard()
{}

void RTE0616Pedalboard::setup()
{
	_hwInterface.setup();

	MIDI.begin(MIDI_CHANNEL_OMNI);

	// No std::bind so lambda.
	MIDI.setHandleSystemExclusive([&_sysexInterface](uint8_t* array, unsigned size) {
		_sysexInterface.handleSysEx(array, size);
	});
}

void RTE0616Pedalboard::loop()
{
	_hwInterface.loop();

	process(_hwInterface.getSegmentedValues());

	if (_sysexInterface.shouldSendMeasures())
		_sysexInterface.sendMesures(_hwInterface.getRawValues(), NB_PIN);

	MIDI.read();
}

void RTE0616Pedalboard::process(int8_t* pinSegment)
{
	pdlbrd_key_t newKey = PDLBRD_NO_KEY_PRESSED;
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

	if (currentKey != newKey) {
		if (currentKey != PDLBRD_NO_KEY_PRESSED)
			MIDI.sendNoteOff((_midiConfig.getOctave()*TONES_IN_OCTAVE)+currentKey, 0, _midiConfig.getChannel());
		if (newKey != PDLBRD_NO_KEY_PRESSED)
			MIDI.sendNoteOn((_midiConfig.getOctave()*TONES_IN_OCTAVE)+newKey, velocity, _midiConfig.getChannel());
		currentKey = newKey;
	}
}


