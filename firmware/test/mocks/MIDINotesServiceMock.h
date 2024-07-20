#ifndef MEMORY_SERVICE_MOCK_H
#define MEMORY_SERVICE_MOCK_H

#include "IMIDINotesService.h"

#include <gmock/gmock.h>

class MIDINotesServiceMock : public IMIDINotesService {
  public:
    MIDINotesServiceMock() = default;
    virtual ~MIDINotesServiceMock() = default;

    MOCK_METHOD(void, setup, ());

    MOCK_METHOD(void, loop, ());

    MOCK_METHOD(void, reset, ());

};

#endif