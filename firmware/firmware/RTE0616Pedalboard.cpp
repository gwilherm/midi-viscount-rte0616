#include "RTE0616Pedalboard.h"

#include <Arduino.h>
#include "AMIDIInterface.h"
#include "CalibrationConfig.h"
#include "HardwareInterface.h"
#include "MIDIConfig.h"
#include "MIDISysexInterface.h"
#include "USBMIDIInterface.h"
#include "pdlbrdkeys.h"


#define TONES_IN_OCTAVE 12


pdlbrd_key_t currentKey = PDLBRD_NO_KEY_PRESSED;


MidiConfig RTE0616Pedalboard::_midiConfig;
CalibrationConfig RTE0616Pedalboard::_calibrationConfig;
HardwareInterface RTE0616Pedalboard::_hwInterface(RTE0616Pedalboard::_calibrationConfig);
USBMIDIInterface RTE0616Pedalboard::_midiInterface;
MIDISysexInterface RTE0616Pedalboard::_sysexInterface(RTE0616Pedalboard::_midiInterface, RTE0616Pedalboard::_midiConfig, RTE0616Pedalboard::_calibrationConfig);

RTE0616Pedalboard::RTE0616Pedalboard()
{}

void RTE0616Pedalboard::setup()
{
	_hwInterface.setup();
	_midiInterface.setup();

	// No std::bind so lambda.
	_midiInterface.setHandleSystemExclusive([&_sysexInterface](uint8_t* array, unsigned size) {
		_sysexInterface.handleSysEx(array, size);
	});
}

void RTE0616Pedalboard::loop()
{
	_hwInterface.loop();

	process(_hwInterface.getSegmentedValues());

	if (_sysexInterface.shouldSendMeasures())
		_sysexInterface.sendMeasures(_hwInterface.getRawValues(), NB_PIN);

	_midiInterface.loop();
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
			_midiInterface.sendNoteOff((_midiConfig.getOctave()*TONES_IN_OCTAVE)+currentKey, _midiConfig.getChannel());
		if (newKey != PDLBRD_NO_KEY_PRESSED)
			_midiInterface.sendNoteOn((_midiConfig.getOctave()*TONES_IN_OCTAVE)+newKey, _midiConfig.getChannel());
		currentKey = newKey;
	}
}


