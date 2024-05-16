#include "RTE0616Pedalboard.h"

RTE0616Pedalboard gPedalboard;

void setup()
{
	Serial.begin(9600);

	gPedalboard.setup();
}

void loop()
{
	gPedalboard.loop();
}