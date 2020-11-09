#ifndef STATICRASTER_H
#define STATICRASTER_H

#include <limits>
#include <vector>

using namespace std;

class Raster
{
public:
    Raster();

    void setSize(int width, int height); //clears the map, and creates a new map with the new size
    void setCell(int x, int y, int value);
    void setMinValue(int value);
    void setMaxValue(int value);

    int getCell(int x, int y);
    int getMinValue();
    int getMaxValue();

    unsigned int getWidth();
    unsigned int getHeight();

private:
    vector<vector<int>> map;
    unsigned int width, heigth;
    int minValue, maxValue;
};

#endif // STATICRASTER_H
