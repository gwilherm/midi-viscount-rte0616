#include "MIDISysexInterface.h"
#include "CalibrationConfig.h"

#include <stdint.h>

#define MANU_ID 0x31 			// Viscount Manufacturer ID
#define PRODUCT_ID 0x06, 0x16	// for RTE0616
#define DEVICE_ID MANU_ID, PRODUCT_ID

#define MIN_MIDI_CHANNEL 1
#define MAX_MIDI_CHANNEL 16

#define MIN_OCTAVE 0
#define MAX_OCTAVE 6

uint8_t SYSEX_REQ_PREFIX[] = { 0xf0, DEVICE_ID };
uint8_t SYSEX_IDENT_REQ[]  = { 0xf0, 0x7e, 0x7f, 0x06, 0x01, 0xf7 }; // f1,      f2,     p1, p2,   v1,   v2,   v3,   v4
uint8_t SYSEX_IDENT_RES[]  = {       0x7e, 0x01, 0x06, 0x02, MANU_ID, 0x00, MANU_ID, PRODUCT_ID, 0x00, 0x00, 0x00, 0x00 };

typedef enum {
	SUBCMD_GET = 1,
	SUBCMD_SET,
	SUBCMD_PUSH,
	SUBCMD_MAX
} pdlbrd_sysex_sub_cmd_t;

bool isArrayEqual(const uint8_t* a, const uint8_t* b, const unsigned size)
{
	for (unsigned i = 0; i < size; i++)
		if (a[i] != b[i])
			return false;

	return true;
}

MIDISysexInterface::MIDISysexInterface(AMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, CalibrationConfig& calibrationConfig):
	_usbMidiInterface(usbMidiInterface),
	_midiConfig(midiConfig),
	_calibrationConfig(calibrationConfig),
	_shouldSendMeasures(false)
{
    initSysexIdentResponse();
}

void MIDISysexInterface::initSysexIdentResponse()
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

void MIDISysexInterface::handleSysEx(uint8_t* array, unsigned size)
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
        _usbMidiInterface.sendSysEx(sizeof(SYSEX_IDENT_RES), SYSEX_IDENT_RES);
    }
    else if ((size > sizeof(SYSEX_REQ_PREFIX) && isArrayEqual(array, SYSEX_REQ_PREFIX, sizeof(SYSEX_REQ_PREFIX))))
    {
		int offset = sizeof(SYSEX_REQ_PREFIX);
		handleCommand(static_cast<pdlbrd_sysex_cmd_t>(array[offset]), &array[offset+1], size - (offset+2));
	}
}


void MIDISysexInterface::handleMeasuresRequest(const pdlbrd_measures_request_t measuresRequest)
{
#if defined (FW_DEBUG)
		Serial.print("Handle Measures Request ");
		Serial.println(measuresRequest);
#endif

	switch (measuresRequest) {
	case MEASURES_ON:
		_shouldSendMeasures = true;
		break;
	case MEASURES_OFF:
	default:
		_shouldSendMeasures = false;
		break;
	}
}

void MIDISysexInterface::handleGetConfiguration()
{
#if defined (FW_DEBUG)
	Serial.println("handleGetConfiguration");
#endif
	uint8_t data[] = { DEVICE_ID, CMD_CONFIGURATION, SUBCMD_GET, _midiConfig.getChannel(), _midiConfig.getOctave() };
	_usbMidiInterface.sendSysEx(sizeof(data), data);
}

void MIDISysexInterface::handleSetConfiguration(uint8_t* data)
{
#if defined (FW_DEBUG)
	Serial.println("handleSetConfiguration");
#endif

	if ((data[0] >= MIN_MIDI_CHANNEL) && (data[0] <= MAX_MIDI_CHANNEL))
		_midiConfig.setChannel(data[0]);

	if ((data[1] >= MIN_OCTAVE) && (data[1] <= MAX_OCTAVE))
		_midiConfig.setOctave(data[1]);

	// Acknowledge
	handleGetConfiguration();
}

void MIDISysexInterface::handleGetCalibration()
{
#if defined (FW_DEBUG)
	Serial.println("handleGetCalibration");
#endif
	//                                                             MARGIN       VSEG1       VSEG2       VSEG3       VSEG4
	uint8_t data[] = { DEVICE_ID, CMD_CALIBRATION, SUBCMD_GET, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	data[5] = (_calibrationConfig.getMargin() >> 7) & 0x7F;
	data[6] =  _calibrationConfig.getMargin() & 0x7F;

	for (int i = 0; i < 4; i++) {
		data[7+(i*2)] = (_calibrationConfig.getVSeg(i) >> 7) & 0x7F;
		data[8+(i*2)] =  _calibrationConfig.getVSeg(i) & 0x7F;
	}

	_usbMidiInterface.sendSysEx(sizeof(data), data);
}

void MIDISysexInterface::handleSetCalibration(uint8_t* data)
{
#if defined (FW_DEBUG)
	Serial.println("handlesetCalibration");
#endif

	_calibrationConfig.setMargin(((data[0] << 7) & 0x7F) + (data[1] & 0x7F));

	for (int i = 0; i < 4; i++) {
		_calibrationConfig.setVSeg(i, ((data[2+(i*2)] & 0x7F) << 7) + (data[3+(i*2)] & 0x7F));
#if defined (FW_DEBUG)
	Serial.print("SEG");
	Serial.print(i+1);
	Serial.print(": ");
	Serial.println(_calibrationConfig.getVSeg(i));
#endif
	}

	// Acknowledge
	handleGetCalibration();
}

void MIDISysexInterface::handleCommand(const pdlbrd_sysex_cmd_t cmd, uint8_t* data, int data_size)
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
	case CMD_MEASURES_REQUEST:
		if (data_size > 0)
			handleMeasuresRequest(static_cast<pdlbrd_measures_request_t>(data[0]));
		break;
	default:
#if defined (FW_DEBUG)
		Serial.println("Unknown command ");
		Serial.println(cmd);
#endif
		break;
	}
}

void MIDISysexInterface::sendMeasures(int* val, int nbVal)
{
	uint8_t bytes[5+(nbVal*2)] = { DEVICE_ID, CMD_MEASURES, SUBCMD_PUSH };
	for (int i = 0; i < nbVal; i++)
	{
		bytes[5+(i*2)] = (val[i] >> 7) & 0x7F;
		bytes[6+(i*2)] =  val[i] & 0x7F;
	}
	_usbMidiInterface.sendSysEx(sizeof(bytes), bytes);
}