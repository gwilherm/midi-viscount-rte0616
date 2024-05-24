#ifndef PEDAL_INTERFACE_MOCK_H
#define PEDAL_INTERFACE_MOCK_H

#include "IPedalInterface.h"

#include <gmock/gmock.h>

class PedalInterfaceMock : public IPedalInterface {
  public:
    PedalInterfaceMock() = default;
    virtual ~PedalInterfaceMock() = default;

    MOCK_METHOD(uint32_t, getExpressionValue, ());

    MOCK_METHOD(bool, getSwitchState, (int));
};

#endif