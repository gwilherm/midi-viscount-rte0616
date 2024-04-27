#include <USB-MIDI.h>

#include "pdlbrdkeys.h"

USBMIDI_CREATE_DEFAULT_INSTANCE();

uint8_t SYSEX_IDENT_REQ[]  = { 0xf0, 0x7e, 0x7f, 0x06, 0x01, 0xf7 };
uint8_t SYSEX_IDENT_RES[]  = {       0x7e, 0x01, 0x06, 0x02, 0x00, 0x31, 0x00, 0x31, 0x06, 0x16 }; // 0x31: Viscount Manufacturer ID, 0x06 0x16 for RTE0616

#define MIDI_CHANNEL 1

#define TONES_IN_OCTAVE 12
#define DEFAULT_OCTAVE 3
#define MIN_OCTAVE 0
#define MAX_OCTAVE 6

//#define SEG1 933
//#define SEG2 625
//#define SEG3 442
//#define SEG4 130
#define SEG1 830
#define SEG2 512
#define SEG3 326
#define SEG4 14

#define NB_PIN 8
uint8_t pin[NB_PIN] = {A0, A1, A2, A3, A6, A7, A8, A9};

uint8_t octave = DEFAULT_OCTAVE;
uint8_t velocity = 127;
uint8_t channel = MIDI_CHANNEL;

pdlbrd_key_t currentKey = PDLBRD_KEY_NOT_PRESSED;
pdlbrd_key_t newKey = currentKey;

void handleSysEx(uint8_t* array, unsigned size);

bool approxEquals(int32_t ref, int32_t val);
bool isArrayEqual(const uint8_t* a, const uint8_t* b, const unsigned size);

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);

	for (int i = 0; i < NB_PIN; i++)
		pinMode(pin[i], INPUT_PULLUP);

	MIDI.begin(MIDI_CHANNEL_OMNI);
	MIDI.setHandleSystemExclusive(handleSysEx);
}

void loop() {
	int val[NB_PIN] = {0};
	for (int i = 0; i < NB_PIN; i++)
    {
		val[i] = analogRead(pin[i]);
#if defined (FW_DEBUG)
		Serial.print(val[i]);
		Serial.print(" ");
	}
	Serial.println();
#else
    }
#endif

	if (approxEquals(SEG4, val[2]))
		newKey = PDLBRD_KEY_D3;
	else if (approxEquals(SEG4, val[1]))
		newKey = PDLBRD_KEY_CS3;
	else if (approxEquals(SEG4, val[0]))
		newKey = PDLBRD_KEY_C3;
	else if (approxEquals(SEG3, val[7]))
		newKey = PDLBRD_KEY_B2;
	else if (approxEquals(SEG3, val[6]))
		newKey = PDLBRD_KEY_AS2;
	else if (approxEquals(SEG3, val[5]))
		newKey = PDLBRD_KEY_A2;
	else if (approxEquals(SEG3, val[4]))
		newKey = PDLBRD_KEY_GS2;
	else if (approxEquals(SEG3, val[3]))
		newKey = PDLBRD_KEY_G2;
	else if (approxEquals(SEG3, val[2]))
		newKey = PDLBRD_KEY_FS2;
	else if (approxEquals(SEG3, val[1]))
		newKey = PDLBRD_KEY_F2;
	else if (approxEquals(SEG3, val[0]))
		newKey = PDLBRD_KEY_E2;
	else if (approxEquals(SEG2, val[7]))
		newKey = PDLBRD_KEY_DS2;
	else if (approxEquals(SEG2, val[6]))
		newKey = PDLBRD_KEY_D2;
	else if (approxEquals(SEG2, val[5]))
		newKey = PDLBRD_KEY_CS2;
	else if (approxEquals(SEG2, val[4]))
		newKey = PDLBRD_KEY_C2;
	else if (approxEquals(SEG2, val[3]))
		newKey = PDLBRD_KEY_B1;
	else if (approxEquals(SEG2, val[2]))
		newKey = PDLBRD_KEY_AS1;
	else if (approxEquals(SEG2, val[1]))
		newKey = PDLBRD_KEY_A1;
	else if (approxEquals(SEG2, val[0]))
		newKey = PDLBRD_KEY_GS1;
	else if (approxEquals(SEG1, val[7]))
		newKey = PDLBRD_KEY_G1;
	else if (approxEquals(SEG1, val[6]))
		newKey = PDLBRD_KEY_FS1;
	else if (approxEquals(SEG1, val[5]))
		newKey = PDLBRD_KEY_F1;
	else if (approxEquals(SEG1, val[4]))
		newKey = PDLBRD_KEY_E1;
	else if (approxEquals(SEG1, val[3]))
		newKey = PDLBRD_KEY_DS1;
	else if (approxEquals(SEG1, val[2]))
		newKey = PDLBRD_KEY_D1;
	else if (approxEquals(SEG1, val[1]))
		newKey = PDLBRD_KEY_CS1;
	else if (approxEquals(SEG1, val[0]))
		newKey = PDLBRD_KEY_C1;
	else
		newKey = PDLBRD_KEY_NOT_PRESSED;

#if defined (FW_DEBUG)
    Serial.println(PDLBRD_KEY_STR[newKey]);
#endif

	if (currentKey != newKey) {
		if (currentKey != PDLBRD_KEY_NOT_PRESSED)
			MIDI.sendNoteOff((octave*TONES_IN_OCTAVE)+currentKey, 0, channel);
		if (newKey != PDLBRD_KEY_NOT_PRESSED)
			MIDI.sendNoteOn((octave*TONES_IN_OCTAVE)+newKey, velocity, channel);
		currentKey = newKey;
	}

	MIDI.read();
}

void handleSysEx(uint8_t* array, unsigned size)
{
#if defined (FW_DEBUG)
    Serial.println("Handle SysEx");
    for (unsigned i = 0; i < size; i++)
    {
        Serial.print(array[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
#endif

    if ((size == sizeof(SYSEX_IDENT_REQ)) && isArrayEqual(array, SYSEX_IDENT_REQ, size))
    {
        MIDI.sendSysEx(sizeof(SYSEX_IDENT_RES), SYSEX_IDENT_RES);
    }
}

bool approxEquals(int32_t ref, int32_t val)
{
	return ((val >= ref-50) && (val <= ref+50));
}

bool isArrayEqual(const uint8_t* a, const uint8_t* b, const unsigned size)
{
    for (unsigned i = 0; i < size; i++)
        if (a[i] != b[i])
            return false;

    return true;
}
