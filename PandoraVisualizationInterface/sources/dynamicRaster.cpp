#include "dynamicRaster.h"

DynamicRaster::DynamicRaster(int numSteps){
    this->numSteps = numSteps;
    minValue = numeric_limits<int>::max();
    maxValue = numeric_limits<int>::min();
    map = vector<Raster*>(numSteps);
}

DynamicRaster::~DynamicRaster(){
    for (unsigned int i = 0; i < numSteps; ++i){
        delete map[i];
    }
}

void DynamicRaster::setSize(int width, int height){
    this->width = width;
    this->heigth = height;
}


void DynamicRaster::setRaster(int step, Raster* r){
    map[step] = r;
}

void DynamicRaster::setMinValue(int value){
    minValue = value;
}

void DynamicRaster::setMaxValue(int value){
    maxValue = value;
}

Raster* DynamicRaster::getRaster(int step){
    return map[step];
}

int DynamicRaster::getMinValue(){
    return minValue;
}

int DynamicRaster::getMaxValue(){
    return maxValue;
}


unsigned int DynamicRaster::getWidth(){
    return width;
}

unsigned int DynamicRaster::getHeight(){
    return heigth;
}

unsigned int DynamicRaster::getNumSteps(){
    return numSteps;
}
