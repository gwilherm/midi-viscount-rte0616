#include "MIDICCService.h"
#include "PedalInterface.h"

MIDICCService::MIDICCService(AMIDIInterface& usbMidiInterface, MidiConfig& midiConfig, PedalInterface& pedalInterface) :
    _usbMidiInterface(usbMidiInterface),
	_midiConfig(midiConfig),
    _pedalInterface(pedalInterface)
{}

void MIDICCService::loop()
{
    Serial.print("Expression: ");
    Serial.println(_pedalInterface.getExpressionValue());
    Serial.print("Sw: ");
    for (int i = 0; i < NB_SWITCH; i++)
    {
        Serial.print(_pedalInterface.getSwitchState(i));
        Serial.print(" ");
    }
    Serial.println();
}