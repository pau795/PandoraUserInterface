#ifndef DINAMICRASTER_H
#define DINAMICRASTER_H

#include "raster.h"

class DynamicRaster
{
public:

    DynamicRaster(int numSteps);
    ~DynamicRaster();

    void setSize(int width, int height);
    void setRaster(int step, Raster *r);
    void setMinValue(int value);
    void setMaxValue(int value);

    Raster* getRaster(int step);
    int getMinValue();
    int getMaxValue();
    unsigned int getNumSteps();

    unsigned int getWidth();
    unsigned int getHeight();

private:
    vector<Raster*> map;
    unsigned int numSteps;
    unsigned int width, heigth;
    int minValue, maxValue;


};

#endif // DINAMICRASTER_H
