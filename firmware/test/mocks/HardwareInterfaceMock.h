#ifndef HARDWARE_INTERFACE_MOCK_H
#define HARDWARE_INTERFACE_MOCK_H

#include "IHardwareInterface.h"

#include <gmock/gmock.h>

class HardwareInterfaceMock : public IHardwareInterface {
  public:
    HardwareInterfaceMock() = default;
    virtual ~HardwareInterfaceMock() = default;

    MOCK_METHOD(int*, getRawValues, ());

    MOCK_METHOD(int8_t*, getSegmentedValues, ());
};

#endif