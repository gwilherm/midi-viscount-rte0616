#include <USB-MIDI.h>

#include "pdlbrdkeys.h"

USBMIDI_CREATE_DEFAULT_INSTANCE();

#define MANU_ID 0x31 			// Viscount Manufacturer ID
#define PRODUCT_ID 0x06, 0x16	// for RTE0616
#define DEVICE_ID MANU_ID, PRODUCT_ID

uint8_t SYSEX_REQ_PREFIX[] = { 0xf0, DEVICE_ID };
uint8_t SYSEX_IDENT_REQ[]  = { 0xf0, 0x7e, 0x7f, 0x06, 0x01, 0xf7 }; // f1,      f2,     p1, p2,   v1,   v2,   v3,   v4
uint8_t SYSEX_IDENT_RES[]  = {       0x7e, 0x01, 0x06, 0x02, MANU_ID, 0x00, MANU_ID, PRODUCT_ID, 0x00, 0x00, 0x00, 0x00 };
																	

typedef enum {
	CMD_CONFIGURATION = 1,
	CMD_CALIBRATION,
	CMD_MEASURES,
	CMD_CHANGE_MODE,
	CMD_MAX
} pdlbrd_sysex_cmd_t;

typedef enum {
	SUBCMD_GET = 1,
	SUBCMD_SET,
	SUBCMD_PUSH,
	SUBCMD_MAX
} pdlbrd_sysex_sub_cmd_t;

typedef enum {
	MODE_STANDARD = 1,
	MODE_MEASURE,
	MODE_MAX
} pdlbrd_mode_t;

#define DEFAULT_MIDI_CHANNEL 1
#define DEFAULT_OCTAVE 3

#define MIN_MIDI_CHANNEL 1
#define MAX_MIDI_CHANNEL 16
#define TONES_IN_OCTAVE 12
#define MIN_OCTAVE 0
#define MAX_OCTAVE 6

#define NB_SEGMENT 4
#define DEFAULT_VSEG1 933
#define DEFAULT_VSEG2 625
#define DEFAULT_VSEG3 442
#define DEFAULT_VSEG4 130
// #define DEFAULT_VSEG1 830
// #define DEFAULT_VSEG2 512
// #define DEFAULT_VSEG3 326
// #define DEFAULT_VSEG4 14
#define DEFAULT_MARGIN 50

uint8_t pin[NB_PIN] = {A0, A1, A2, A3, A6, A7, A8, A9};

uint8_t channel = DEFAULT_MIDI_CHANNEL;
uint8_t octave = DEFAULT_OCTAVE;
uint8_t velocity = 127;
uint16_t vSeg[] = { DEFAULT_VSEG1, DEFAULT_VSEG2, DEFAULT_VSEG3, DEFAULT_VSEG4 };
uint16_t margin = DEFAULT_MARGIN;

int8_t pinSegment[NB_PIN] = {	PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG,
								PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG };
int8_t lastPinSegment[NB_PIN] = {	PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG,
									PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG };
unsigned long lastTick[NB_PIN] = {};

pdlbrd_key_t currentKey = PDLBRD_NO_KEY_PRESSED;

pdlbrd_mode_t currentMode = MODE_STANDARD;


void handleSysEx(uint8_t* array, unsigned size);

bool approxEquals(int32_t ref, int32_t val);
bool isArrayEqual(const uint8_t* a, const uint8_t* b, const unsigned size);
void initSysexIdentResponse();
void processStandardMode(int* val);
void processMeasureMode(int* val);

void setup()
{
	Serial.begin(9600);

	initSysexIdentResponse();

	for (int i = 0; i < NB_PIN; i++)
		pinMode(pin[i], INPUT_PULLUP);

	MIDI.begin(MIDI_CHANNEL_OMNI);
	MIDI.setHandleSystemExclusive(handleSysEx);
}

void loop()
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

	processStandardMode(val);

	if (currentMode == MODE_MEASURE)
		processMeasureMode(val);

	MIDI.read();
}

void processStandardMode(int* val)
{
	pdlbrd_key_t newKey = PDLBRD_NO_KEY_PRESSED;
	int8_t newPinSegment[NB_PIN] = { 	PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG,
										PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG };
	for (uint8_t pin = 0; pin < NB_PIN; pin++)
	{
		for (uint8_t seg = PDLBRD_SEG_1; seg < PDLBRD_NB_SEG; seg++)
		{
			if (approxEquals(vSeg[seg], val[pin]))
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
	Serial.println();

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
			MIDI.sendNoteOff((octave*TONES_IN_OCTAVE)+currentKey, 0, channel);
		if (newKey != PDLBRD_NO_KEY_PRESSED)
			MIDI.sendNoteOn((octave*TONES_IN_OCTAVE)+newKey, velocity, channel);
		currentKey = newKey;
	}
}

void initSysexIdentResponse()
{
	char* fwVersionStr = strdup(FW_VERSION);
#if defined (FW_DEBUG)
	Serial.print("Fw version: ");
	Serial.println(fwVersionStr);
#endif

	// Major: 1 byte
	int tmp = atoi(strtok(fwVersionStr, "."));
    SYSEX_IDENT_RES[9] = tmp & 0xF7;

	// Minor: 1 byte
	tmp = atoi(strtok(NULL, "."));
    SYSEX_IDENT_RES[10] = tmp & 0xF7;

	// Patch: two bytes
	tmp = atoi(strtok(NULL, "."));
    SYSEX_IDENT_RES[11] = (tmp >> 7) & 0x7F; // MSB
    SYSEX_IDENT_RES[12] = tmp & 0x7F;        // LSB
}

void processMeasureMode(int* val)
{
	uint8_t bytes[5+(8*2)] = { DEVICE_ID, CMD_MEASURES, SUBCMD_PUSH };
	for (int i = 0; i < 8; i++)
	{
		bytes[5+(i*2)] = (val[i] >> 7) & 0x7F;
		bytes[6+(i*2)] =  val[i] & 0x7F;
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

void handleGetConfiguration()
{
#if defined (FW_DEBUG)
	Serial.println("handleGetConfiguration");
#endif
	uint8_t data[] = { DEVICE_ID, CMD_CONFIGURATION, SUBCMD_GET, channel, octave };
	MIDI.sendSysEx(sizeof(data), data);
}

void handleSetConfiguration(uint8_t* data)
{
#if defined (FW_DEBUG)
	Serial.println("handleSetConfiguration");
#endif

	if ((data[0] >= MIN_MIDI_CHANNEL) && (data[0] <= MAX_MIDI_CHANNEL))
		channel = data[0];

	if ((data[1] >= MIN_OCTAVE) && (data[0] <= MAX_OCTAVE))
		octave = data[1];

	// Acknowledge
	handleGetConfiguration();
}

void handleGetCalibration()
{
#if defined (FW_DEBUG)
	Serial.println("handleGetCalibration");
#endif
	//                                                             MARGIN       VSEG1       VSEG2       VSEG3       VSEG4
	uint8_t data[] = { DEVICE_ID, CMD_CALIBRATION, SUBCMD_GET, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
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

	// Acknowledge
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
	case CMD_CONFIGURATION:
		if (data_size > 0)
		{
			if (data[0] == SUBCMD_GET)
				handleGetConfiguration();
			if ((data[0] == SUBCMD_SET) && (data_size == 3))
				handleSetConfiguration(&data[1]);
		}
		break;
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
    else if ((size > sizeof(SYSEX_REQ_PREFIX) && isArrayEqual(array, SYSEX_REQ_PREFIX, sizeof(SYSEX_REQ_PREFIX))))
    {
		int offset = sizeof(SYSEX_REQ_PREFIX);
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
