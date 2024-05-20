#ifndef CALIBRATION_CONFIG_H
#define CALIBRATION_CONFIG_H

#include <Arduino.h>

#define NB_VSEG 4
// #define DEFAULT_VSEG1 933
// #define DEFAULT_VSEG2 625
// #define DEFAULT_VSEG3 442
// #define DEFAULT_VSEG4 130
#define DEFAULT_VSEG1 830
#define DEFAULT_VSEG2 512
#define DEFAULT_VSEG3 326
#define DEFAULT_VSEG4 14
#define DEFAULT_MARGIN 50

class CalibrationConfig
{
public:
    CalibrationConfig() : _margin(DEFAULT_MARGIN), _vSeg{ DEFAULT_VSEG1, DEFAULT_VSEG2, DEFAULT_VSEG3, DEFAULT_VSEG4 } {}
    virtual ~CalibrationConfig() = default;

    inline uint16_t getMargin() { return _margin; };
    inline void setMargin(uint16_t m) { _margin = m; };
    inline uint16_t getVSeg(int i) { return ((i >= 0) && (i < NB_VSEG))? _vSeg[i] : 0; }
    inline void setVSeg(int i, uint16_t v) { if ((i >= 0) && (i < NB_VSEG)) _vSeg[i] = v; }


private:
    uint16_t _margin;
    uint16_t _vSeg[NB_VSEG];
};

void CalibrationConfig::reset()
{
    _margin = DEFAULT_MARGIN;
    _vSeg[0] = DEFAULT_VSEG1;
    _vSeg[1] = DEFAULT_VSEG2;
    _vSeg[2] = DEFAULT_VSEG3;
    _vSeg[3] = DEFAULT_VSEG4;
}
#endif