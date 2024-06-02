#ifndef I_MEMORY_SERVICE_H
#define I_MEMORY_SERVICE_H

class IMemoryService
{
public:
    virtual void restore() = 0;
    virtual void store() = 0;
    virtual void factoryReset() = 0;

    virtual void updateMidiConfig() = 0;
    virtual void updateCalibration() = 0;
};

#endif