#include "MIDICCService.h"

MIDICCService::MIDICCService(IMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, IExtraPedalInterface& pedalInterface) :
    _usbMidiInterface(usbMidiInterface),
	_midiConfig(midiConfig),
    _extraPedalInterface(pedalInterface),
    _lastExpressionValue(0),
    _lastSustainState(false)
{}

void MIDICCService::loop()
{
    uint8_t expressionValue = _extraPedalInterface.getExpressionValue() / 8;
    if (expressionValue > MIDI_MAXVAL) expressionValue = MIDI_MAXVAL;
    if (expressionValue != _lastExpressionValue)
    {
        _usbMidiInterface.sendCC(IMIDIInterface::CC_EXPRESSION, expressionValue, _midiConfig.getChannel());
        _lastExpressionValue = expressionValue;
    }

    bool sustainState = _extraPedalInterface.getSwitchState(0);
    if (sustainState != _lastSustainState)
    {
        _usbMidiInterface.sendCC(IMIDIInterface::CC_SUSTAIN, (sustainState? MIDI_MAXVAL:0), _midiConfig.getChannel());
        _lastSustainState = sustainState;
    }
}