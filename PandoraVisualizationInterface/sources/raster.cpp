#include "raster.h"

Raster::Raster(){

}

void Raster::setSize(int width, int height){
    this->width = width;
    this->heigth = height;
    minValue = numeric_limits<int>::max();
    maxValue = numeric_limits<int>::min();
    map.clear();
    map = vector<vector<int>>(width, vector<int>(height));
}

void Raster::setCell(int x, int y, int value){
    map[x][y] = value;
}

void Raster::setMinValue(int value){
    minValue = value;
}

void Raster::setMaxValue(int value){
    maxValue = value;
}

int Raster::getCell(int x, int y){
    return map[x][y];
}

int Raster::getMinValue(){
    return minValue;
}

int Raster::getMaxValue(){
    return maxValue;
}


unsigned int Raster::getWidth(){
    return width;
}

unsigned int Raster::getHeight(){
    return heigth;
}
