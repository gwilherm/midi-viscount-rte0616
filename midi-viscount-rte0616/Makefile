BOARD_TAG    = micro

# Platform libs
ARDUINO_LIBS = SPI
# User libs
ARDUINO_LIBS+= MIDIUSB MIDI_Library USB-MIDI

# Workaround for MIDIUSB with 1.8.19: 1819 < 10606 (1.6.6)
ARDUINO_VERSION = 10819

# Firmware version
FW_VERSION = $(shell git describe --always --tags)

CPPFLAGS = -DUSB_PRODUCT='"Viscount RTE0616"' -DFW_VERSION='"$(FW_VERSION)"'
CPPFLAGS+= -DFW_DEBUG

include /usr/share/arduino/Arduino.mk