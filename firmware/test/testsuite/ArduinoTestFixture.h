#ifndef TEST_FIXTURE_H
#define TEST_FIXTURE_H

#include "ArduinoMock.h"

class ArduinoTestFixture: public ::testing::Test{
public:
    ArduinoTestFixture(){
        _arduinoMock.reset(new ::testing::NiceMock<ArduinoMock>());
    }
    ~ArduinoTestFixture(){
        _arduinoMock.reset();
    }
    virtual void SetUp(){}
    virtual void TearDown(){}

    // pointer for accessing mocked library
    static std::unique_ptr<::testing::NiceMock<ArduinoMock>> _arduinoMock;
};

#endif