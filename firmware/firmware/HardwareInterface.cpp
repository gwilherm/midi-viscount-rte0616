#include "HardwareInterface.h"

HardwareInterface::HardwareInterface(CalibrationConfig& calibrationConfig):
	_calibrationConfig(calibrationConfig)
{}

void HardwareInterface::setup()
{
	for (int i = 0; i < NB_PIN; i++)
		pinMode(_pin[i], INPUT_PULLUP);
}

void HardwareInterface::loop()
{
	read();
	process();
}

void HardwareInterface::read()
{
	for (int i = 0; i < NB_PIN; i++)
	{
		_rawValues[i] = analogRead(_pin[i]);
#if defined (FW_DEBUG_VERBOSE_HWINT_READ)
		Serial.print(_rawValues[i]);
		Serial.print(" ");
	}
	Serial.println();
#else
	}
#endif
}

void HardwareInterface::process()
{
	int8_t newPinSegment[NB_PIN] = { 	PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG,
										PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG, PDLBRD_NO_SEG };
	for (uint8_t pin = 0; pin < NB_PIN; pin++)
	{
		for (uint8_t seg = PDLBRD_SEG_1; seg < PDLBRD_NB_SEG; seg++)
		{
			if (approxEquals(_calibrationConfig.getVSeg(seg), _rawValues[pin]))
			{
				newPinSegment[pin] = seg;
			}
		}

		if (newPinSegment[pin] != _lastPinSegment[pin])
			_lastTick[pin] = millis();

		if ((millis() - _lastTick[pin]) > 50)
		{
			if (_pinSegment[pin] != newPinSegment[pin])
				_pinSegment[pin] = newPinSegment[pin];
		}

#if defined (FW_DEBUG_VERBOSE_HWINT_PROCESS)
	    Serial.print(_pinSegment[pin]);
	    Serial.print(" ");
#endif
		_lastPinSegment[pin] = newPinSegment[pin];
	}
#if defined (FW_DEBUG_VERBOSE_HWINT_PROCESS)
	Serial.println();
#endif

}

bool HardwareInterface::approxEquals(int32_t ref, int32_t val)
{
	return ((val >= ref - _calibrationConfig.getMargin()) && (val <= ref + _calibrationConfig.getMargin()));
}