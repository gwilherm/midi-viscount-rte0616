#ifndef MEMORY_SERVICE_MOCK_H
#define MEMORY_SERVICE_MOCK_H

#include "IMemoryService.h"

#include <gmock/gmock.h>

class MemoryServiceMock : public IMemoryService {
  public:
    MemoryServiceMock() = default;
    virtual ~MemoryServiceMock() = default;

    MOCK_METHOD(void, restore, ());

    MOCK_METHOD(void, store, ());

    MOCK_METHOD(void, factoryReset, ());

    MOCK_METHOD(void, updateMidiConfig, ());

    MOCK_METHOD(void, updateCalibration, ());
};

#endif