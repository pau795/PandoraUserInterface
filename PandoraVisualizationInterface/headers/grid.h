#ifndef GRID_H
#define GRID_H

#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#include "tile.h"
#include "shape.h"
#include "simulationWorld.h"
#include <vector>


using namespace std;

class Grid
{
public:

    Grid(SimulationWorld *world, string currentRaster, map<string, Shape::ShapeSettings> &settings, QOpenGLShaderProgram *program);
    ~Grid();
    void render(glm::mat4 &viewMatrix);
    void setElevation(bool elevation);
    Tile* getTile(int i, int j);

    void setShapeSettings(map<string, Shape::ShapeSettings> &settings);
    void setCurrentRaster(string currentRaster);
    void setCurrentStep(int step);

    static glm::vec3 palette;

private:

    vector<vector<Tile*>> tiles;
    SimulationWorld *world;
    map<string, Shape::ShapeSettings> agentSettings;
    string rasterName;
    Raster* selectedRaster;
    int currentStep;
    int rows, cols;

    QOpenGLShaderProgram *program;

    void initGrid();
    void setShapePosition(SimulationAgent *agent);
    void updateGrid();
    float getCellHeight(int i, int j);

};

#endif // GRID_H
