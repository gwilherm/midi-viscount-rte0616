#ifndef PEDAL_INTERFACE_MOCK_H
#define PEDAL_INTERFACE_MOCK_H

#include "IExtraPedalInterface.h"

#include <gmock/gmock.h>

class ExtraPedalInterfaceMock : public IExtraPedalInterface {
  public:
    ExtraPedalInterfaceMock() = default;
    virtual ~ExtraPedalInterfaceMock() = default;

    MOCK_METHOD(uint32_t, getExpressionValue, ());

    MOCK_METHOD(bool, getSwitchState, (int));
};

#endif