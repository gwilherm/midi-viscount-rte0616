#include "RTE0616Pedalboard.h"

#include <Arduino.h>
#include <USB-MIDI.h>

#include "pdlbrdkeys.h"


USBMIDI_CREATE_DEFAULT_INSTANCE();


#define TONES_IN_OCTAVE 12


uint8_t pin[NB_PIN] = {A0, A1, A2, A3, A6, A7, A8, A9};


uint8_t velocity = 127;


int8_t pinSegment[NB_PIN] = {	PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG,
								PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG };
int8_t lastPinSegment[NB_PIN] = {	PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG,
									PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG };
unsigned long lastTick[NB_PIN] = {};

pdlbrd_key_t currentKey = PDLBRD_NO_KEY_PRESSED;

void handleSysEx(uint8_t* array, unsigned size);

bool approxEquals(int32_t ref, int32_t val);
bool isArrayEqual(const uint8_t* a, const uint8_t* b, const unsigned size);
void process(int* val);


MidiConfig _midiConfig;
CalibrationConfig _calibrationConfig;
MIDISysexInterface _sysexInterface(MIDI, _midiConfig, _calibrationConfig);

RTE0616Pedalboard::RTE0616Pedalboard()
{}

void RTE0616Pedalboard::setup()
{
	for (int i = 0; i < NB_PIN; i++)
		pinMode(pin[i], INPUT_PULLUP);

	MIDI.begin(MIDI_CHANNEL_OMNI);

	// No std::bind so lambda.
	MIDI.setHandleSystemExclusive([&_sysexInterface](uint8_t* array, unsigned size) {
		_sysexInterface.handleSysEx(array, size);
	});
}

void RTE0616Pedalboard::loop()
{
    int val[NB_PIN] = {0};

	for (int i = 0; i < NB_PIN; i++)
	{
		val[i] = analogRead(pin[i]);
#if defined (FW_DEBUG_VERBOSE)
		Serial.print(val[i]);
		Serial.print(" ");
	}
	Serial.println();
#else
	}
#endif

	process(val);

	if (_sysexInterface.shouldSendMeasures())
		_sysexInterface.sendMesures(val, NB_PIN);

	MIDI.read();
}

void RTE0616Pedalboard::process(int* val)
{
	pdlbrd_key_t newKey = PDLBRD_NO_KEY_PRESSED;
	int8_t newPinSegment[NB_PIN] = { 	PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG,
										PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG };
	for (uint8_t pin = 0; pin < NB_PIN; pin++)
	{
		for (uint8_t seg = PDLBRD_SEG_1; seg < PDLBRD_NB_SEG; seg++)
		{
			if (approxEquals(_calibrationConfig.getVSeg(seg), val[pin]))
			{
				newPinSegment[pin] = seg;
			}
		}

		if (newPinSegment[pin] != lastPinSegment[pin])
			lastTick[pin] = millis();

		if ((millis() - lastTick[pin]) > 50)
		{
			if (pinSegment[pin] != newPinSegment[pin])
				pinSegment[pin] = newPinSegment[pin];
		}

#if defined (FW_DEBUG_VERBOSE_2)
	    Serial.print(pinSegment[pin]);
	    Serial.print(" ");
#endif
		lastPinSegment[pin] = newPinSegment[pin];
	}
#if defined (FW_DEBUG_VERBOSE_2)
	Serial.println();
#endif

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

#if defined (FW_DEBUG_VERBOSE_3)
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

bool RTE0616Pedalboard::approxEquals(int32_t ref, int32_t val)
{
	return ((val >= ref - _calibrationConfig.getMargin()) && (val <= ref + _calibrationConfig.getMargin()));
}

