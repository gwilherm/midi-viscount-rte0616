#include "RTE0616Pedalboard.h"

#include <Arduino.h>
#include "AMIDIInterface.h"
#include "CalibrationConfig.h"
#include "HardwareInterface.h"
#include "MIDIConfig.h"
#include "MIDISysexService.h"
#include "PedalInterface.h"
#include "USBMIDIInterface.h"

MidiConfig RTE0616Pedalboard::_midiConfig;
CalibrationConfig RTE0616Pedalboard::_calibrationConfig;
HardwareInterface RTE0616Pedalboard::_hwInterface(RTE0616Pedalboard::_calibrationConfig);
PedalInterface RTE0616Pedalboard::_pedalInterface;

USBMIDIInterface RTE0616Pedalboard::_midiInterface;
MIDISysexService RTE0616Pedalboard::_sysexService(RTE0616Pedalboard::_midiInterface, RTE0616Pedalboard::_midiConfig, RTE0616Pedalboard::_calibrationConfig);
MIDINotesService RTE0616Pedalboard::_notesService(RTE0616Pedalboard::_midiInterface, RTE0616Pedalboard::_midiConfig, RTE0616Pedalboard::_hwInterface);
MIDICCService RTE0616Pedalboard::_ccService(RTE0616Pedalboard::_midiInterface, RTE0616Pedalboard::_midiConfig, RTE0616Pedalboard::_pedalInterface);

RTE0616Pedalboard::RTE0616Pedalboard()
{}

void RTE0616Pedalboard::setup()
{
	_hwInterface.setup();
	_pedalInterface.setup();
	_midiInterface.setup();
	_notesService.setup();
	_ccService.setup();

	// No std::bind so lambda.
	_midiInterface.setHandleSystemExclusive([&_sysexService](uint8_t* array, unsigned size) {
		_sysexService.handleSysEx(array, size);
	});
}

void RTE0616Pedalboard::loop()
{
	_hwInterface.loop();
	_pedalInterface.loop();
	_notesService.loop();
	_ccService.loop();

	if (_sysexService.shouldSendMeasures())
		_sysexService.sendMeasures(_hwInterface.getRawValues(), NB_PIN);

	_midiInterface.loop();
}

