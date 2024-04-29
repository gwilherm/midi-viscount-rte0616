#include <USB-MIDI.h>

#include "pdlbrdkeys.h"

USBMIDI_CREATE_DEFAULT_INSTANCE();

#define MANU_ID 0x31 			// Viscount Manufacturer ID
#define PRODUCT_ID 0x06, 0x16	// for RTE0616

uint8_t SYSEX_IDENT_REQ[]  = { 0xf0, 0x7e, 0x7f, 0x06, 0x01, 0xf7 };
uint8_t SYSEX_IDENT_RES[]  = {       0x7e, 0x01, 0x06, 0x02, 0x00, MANU_ID, 0x00, MANU_ID, PRODUCT_ID };

uint8_t SYSEX_CMD_PREFIX[] = { 0xf0, MANU_ID, PRODUCT_ID };

typedef enum {
	CMD_CALIBRATION = 1,
	CMD_CHANGE_MODE,
	CMD_MAX
} pdlbrd_sysex_cmd_t;

typedef enum {
	SUBCMD_GET = 1,
	SUBCMD_SET,
	SUBCMD_MAX
} pdlbrd_sysex_sub_cmd_t;

typedef enum {
	MODE_STANDARD = 1,
	MODE_MEASURE,
	MODE_MAX
} pdlbrd_mode_t;

#define MIDI_CHANNEL 1

#define TONES_IN_OCTAVE 12
#define DEFAULT_OCTAVE 3
#define MIN_OCTAVE 0
#define MAX_OCTAVE 6

//#define DEFAULT_VSEG1 933
//#define DEFAULT_VSEG2 625
//#define DEFAULT_VSEG3 442
//#define DEFAULT_VSEG4 130
#define DEFAULT_VSEG1 830
#define DEFAULT_VSEG2 512
#define DEFAULT_VSEG3 326
#define DEFAULT_VSEG4 14
#define DEFAULT_MARGIN 50

#define NB_PIN 8
uint8_t pin[NB_PIN] = {A0, A1, A2, A3, A6, A7, A8, A9};

uint8_t octave = DEFAULT_OCTAVE;
uint8_t velocity = 127;
uint8_t channel = MIDI_CHANNEL;
uint16_t vSeg[] = { DEFAULT_VSEG1, DEFAULT_VSEG2, DEFAULT_VSEG3, DEFAULT_VSEG4 };
uint16_t margin = DEFAULT_MARGIN;

pdlbrd_key_t currentKey = PDLBRD_KEY_NOT_PRESSED;
pdlbrd_key_t newKey = currentKey;

pdlbrd_mode_t currentMode = MODE_STANDARD;

void handleSysEx(uint8_t* array, unsigned size);

bool approxEquals(int32_t ref, int32_t val);
bool isArrayEqual(const uint8_t* a, const uint8_t* b, const unsigned size);
void processStandardMode(int* val);
void processMeasureMode(int* val);

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
#if defined (FW_DEBUG_VERBOSE)
		Serial.print(val[i]);
		Serial.print(" ");
	}
	Serial.println();
#else
    }
#endif

	switch (currentMode) {
	case MODE_STANDARD:
		processStandardMode(val);
		break;
	case MODE_MEASURE:
		processMeasureMode(val);
		break;
	default:
		break;
	}

	MIDI.read();
}

void processStandardMode(int* val)
{
	if (approxEquals(vSeg[3], val[2]))
		newKey = PDLBRD_KEY_D3;
	else if (approxEquals(vSeg[3], val[1]))
		newKey = PDLBRD_KEY_CS3;
	else if (approxEquals(vSeg[3], val[0]))
		newKey = PDLBRD_KEY_C3;
	else if (approxEquals(vSeg[2], val[7]))
		newKey = PDLBRD_KEY_B2;
	else if (approxEquals(vSeg[2], val[6]))
		newKey = PDLBRD_KEY_AS2;
	else if (approxEquals(vSeg[2], val[5]))
		newKey = PDLBRD_KEY_A2;
	else if (approxEquals(vSeg[2], val[4]))
		newKey = PDLBRD_KEY_GS2;
	else if (approxEquals(vSeg[2], val[3]))
		newKey = PDLBRD_KEY_G2;
	else if (approxEquals(vSeg[2], val[2]))
		newKey = PDLBRD_KEY_FS2;
	else if (approxEquals(vSeg[2], val[1]))
		newKey = PDLBRD_KEY_F2;
	else if (approxEquals(vSeg[2], val[0]))
		newKey = PDLBRD_KEY_E2;
	else if (approxEquals(vSeg[1], val[7]))
		newKey = PDLBRD_KEY_DS2;
	else if (approxEquals(vSeg[1], val[6]))
		newKey = PDLBRD_KEY_D2;
	else if (approxEquals(vSeg[1], val[5]))
		newKey = PDLBRD_KEY_CS2;
	else if (approxEquals(vSeg[1], val[4]))
		newKey = PDLBRD_KEY_C2;
	else if (approxEquals(vSeg[1], val[3]))
		newKey = PDLBRD_KEY_B1;
	else if (approxEquals(vSeg[1], val[2]))
		newKey = PDLBRD_KEY_AS1;
	else if (approxEquals(vSeg[1], val[1]))
		newKey = PDLBRD_KEY_A1;
	else if (approxEquals(vSeg[1], val[0]))
		newKey = PDLBRD_KEY_GS1;
	else if (approxEquals(vSeg[0], val[7]))
		newKey = PDLBRD_KEY_G1;
	else if (approxEquals(vSeg[0], val[6]))
		newKey = PDLBRD_KEY_FS1;
	else if (approxEquals(vSeg[0], val[5]))
		newKey = PDLBRD_KEY_F1;
	else if (approxEquals(vSeg[0], val[4]))
		newKey = PDLBRD_KEY_E1;
	else if (approxEquals(vSeg[0], val[3]))
		newKey = PDLBRD_KEY_DS1;
	else if (approxEquals(vSeg[0], val[2]))
		newKey = PDLBRD_KEY_D1;
	else if (approxEquals(vSeg[0], val[1]))
		newKey = PDLBRD_KEY_CS1;
	else if (approxEquals(vSeg[0], val[0]))
		newKey = PDLBRD_KEY_C1;
	else
		newKey = PDLBRD_KEY_NOT_PRESSED;

#if defined (FW_DEBUG_VERBOSE)
    Serial.println(PDLBRD_KEY_STR[newKey]);
#endif

	if (currentKey != newKey) {
		if (currentKey != PDLBRD_KEY_NOT_PRESSED)
			MIDI.sendNoteOff((octave*TONES_IN_OCTAVE)+currentKey, 0, channel);
		if (newKey != PDLBRD_KEY_NOT_PRESSED)
			MIDI.sendNoteOn((octave*TONES_IN_OCTAVE)+newKey, velocity, channel);
		currentKey = newKey;
	}
}

void processMeasureMode(int* val)
{
	uint8_t bytes[3+(8*2)] = { 0x31, 0x06, 0x16 };
	for (int i = 0; i < 8; i++)
	{
		bytes[3+(i*2)] = (val[i] >> 7) & 0x7F;
		bytes[4+(i*2)] =  val[i] & 0x7F;
	}
	MIDI.sendSysEx(sizeof(bytes), bytes);
}

void handleChangeMode(const pdlbrd_mode_t mode)
{
	if ((mode > 0) && (mode < MODE_MAX)) {
#if defined (FW_DEBUG)
		Serial.print("Handle change mode ");
		Serial.println(mode);
#endif
		currentMode = mode;
	}
#if defined (FW_DEBUG)
	else {
		Serial.print("Unknown mode ");
		Serial.println(mode);
	}
#endif
}

void handleGetCalibration()
{
#if defined (FW_DEBUG)
	Serial.println("handleGetCalibration");
#endif
	//                                                                     MARGIN       VSEG1       VSEG2       VSEG3       VSEG4
	uint8_t data[] = { MANU_ID, PRODUCT_ID, CMD_CALIBRATION, SUBCMD_GET, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	data[5] = (margin >> 7) & 0x7F;
	data[6] =  margin & 0x7F;

	for (int i = 0; i < 4; i++) {
		data[7+(i*2)] = (vSeg[i] >> 7) & 0x7F;
		data[8+(i*2)] =  vSeg[i] & 0x7F;
	}

	MIDI.sendSysEx(sizeof(data), data);
}

void handleSetCalibration(uint8_t* data)
{
#if defined (FW_DEBUG)
	Serial.println("handlesetCalibration");
#endif

	margin = ((data[0] << 7) & 0x7F) + (data[1] & 0x7F);

	for (int i = 0; i < 4; i++) {
		vSeg[i] = ((data[2+(i*2)] & 0x7F) << 7) + (data[3+(i*2)] & 0x7F);
#if defined (FW_DEBUG)
	Serial.print("SEG");
	Serial.print(i+1);
	Serial.print(": ");
	Serial.println(vSeg[i]);
#endif
	}

	handleGetCalibration();
}

void handleCommand(const pdlbrd_sysex_cmd_t cmd, uint8_t* data, int data_size)
{
#if defined (FW_DEBUG)
		Serial.print("Handle cmd ");
		Serial.print(cmd);
		Serial.print(" data_size: ");
		Serial.println(data_size);
#endif

	switch (cmd) {
	case CMD_CALIBRATION:
		if (data_size > 0)
		{
			if (data[0] == SUBCMD_GET)
				handleGetCalibration();
			if ((data[0] == SUBCMD_SET) && (data_size == 11))
				handleSetCalibration(&data[1]);
		}
		break;
	case CMD_CHANGE_MODE:
		if (data_size > 0)
			handleChangeMode(static_cast<pdlbrd_mode_t>(data[0]));
		break;
	default:
#if defined (FW_DEBUG)
		Serial.println("Unknown command ");
		Serial.println(cmd);
#endif
		break;
	}
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
    else if ((size > sizeof(SYSEX_CMD_PREFIX)) && isArrayEqual(array, SYSEX_CMD_PREFIX, sizeof(SYSEX_CMD_PREFIX)))
    {
		int offset = sizeof(SYSEX_CMD_PREFIX);
		handleCommand(static_cast<pdlbrd_sysex_cmd_t>(array[offset]), &array[offset+1], size - (offset+2));
	}
}

bool approxEquals(int32_t ref, int32_t val)
{
	return ((val >= ref-margin) && (val <= ref+margin));
}

bool isArrayEqual(const uint8_t* a, const uint8_t* b, const unsigned size)
{
	for (unsigned i = 0; i < size; i++)
		if (a[i] != b[i])
			return false;

	return true;
}
