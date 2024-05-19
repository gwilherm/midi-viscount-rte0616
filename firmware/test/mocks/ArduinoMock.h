#ifndef ARDUINO_MOCK_H
#define ARDUINO_MOCK_H

#include <gmock/gmock.h>

class ArduinoMock {
  protected:
    ArduinoMock() = default;

  private:
    static testing::StrictMock<ArduinoMock> *instance;

  public:
    static testing::StrictMock<ArduinoMock> &getInstance();
    static void begin();
    static void end();

    MOCK_METHOD(void, pinMode, (uint8_t, uint8_t));
    MOCK_METHOD(int, analogRead, (uint8_t));

    MOCK_METHOD(unsigned long, millis, ());

    virtual ~ArduinoMock() = default;
};

#endif