#include "grid.h"
#include <iostream>
using namespace std;


glm::vec3 Grid::palette = glm::vec3{1.0,1.0,0.5};


Grid::Grid(SimulationWorld *world, string currentRaster, map<string, Shape::ShapeSettings> &settings, QOpenGLShaderProgram *program){

    this->world = world;
    this->rows = world->getHeight();
    this->cols = world->getWidth();
    this->currentStep = 1;
    this->program = program;
    tiles.resize(rows, vector<Tile*>(cols));
    this->rasterName = currentRaster;
    this->selectedRaster = world->getRaster(rasterName, currentStep);
    this->agentSettings = settings;
    initGrid();
}

Grid::~Grid(){
    for(int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            if (tiles[i][j] != NULL) delete tiles[i][j];
        }
    }
}

void Grid::setShapeSettings(map<string, Shape::ShapeSettings> &settings){
    this->agentSettings = settings;
    for (string type : world->getAgentTypeList()){
        for(string agentName :world->getAgentList(type)){
            SimulationAgent *agent = world->getAgent(type, agentName);
            agent->getShape()->setSettings(settings.at(type));
        }
    }

}
void Grid::setCurrentRaster(string rasterName){
    this->rasterName = rasterName;
    this->selectedRaster = world->getRaster(rasterName, currentStep);
    updateGrid();

}
void Grid::setCurrentStep(int step){
    this->currentStep = step;
    this->selectedRaster = world->getRaster(rasterName, currentStep);
    updateGrid();
}

void Grid::updateGrid(){
    for(int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            float height = getCellHeight(i,j);
            if (tiles[i][j]->getHeight() != height) tiles[i][j]->setHeight(height);
        }
        const vector<SimulationAgent*> &v = world->getAgentsToLoad(currentStep);
        for(SimulationAgent* agent : v){
            setShapePosition(agent);
        }
    }

}

float Grid::getCellHeight(int i, int j){
    pair<int, int> p = world->getRasterMinMaxValues(rasterName);
    int value = selectedRaster->getCell(i, j);
    int minValue = p.first, maxValue = p.second;
    float height;
    if (minValue == maxValue) height = 0.01;
    else height = float(value-minValue) / float(maxValue - minValue) * 0.5;
    return height;
}

Tile* Grid::getTile(int i, int j){
    return tiles[i][j];
}

void Grid::setElevation(bool elevation){
    for(int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            tiles[j][i]->setElevation(elevation);
        }
    }
    for (string type : world->getAgentTypeList()){
        for(string agentName :world->getAgentList(type)){
            SimulationAgent *agent = world->getAgent(type, agentName);
            setShapePosition(agent);
        }
    }
}

void Grid::render(glm::mat4 &viewMatrix){
    for(int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            tiles[j][i]->render(viewMatrix);
        }
    }
    const vector<SimulationAgent*> &v = world->getAgentsToLoad(currentStep);
    for(SimulationAgent* agent : v){
        if (agentSettings.at(agent->getAgentType()).active){
             agent->getShape()->render(viewMatrix);
        }
    }
}


void Grid::setShapePosition(SimulationAgent* agent){
    int x = agent->getInt(currentStep, "x");
    int y = agent->getInt(currentStep, "y");
    agent->getShape()->setPosition(tiles[x][y]->getCenter());
}


void Grid::initGrid(){

    float ratiox = float(rows)/float(cols), ratioz = float(cols)/float(rows);
    float maxRx = glm::max(ratiox, 1.0f), maxRz = glm::max(ratioz, 1.0f);
    for(int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            float xmin = float(j)/cols*10/maxRx, xmax = float(j+1)/cols*10/maxRx, zmin =float(i)/rows*10/maxRz, zmax = float(i+1)/rows*10/maxRz;
            float height =  getCellHeight(i, j);
            Tile* t = new Tile(glm::vec2{xmin, zmin}, glm::vec2{xmax, zmax}, height, program);
            tiles[j][i] = t;
        }
    }
    for (string type : world->getAgentTypeList()){
        for(string agentName :world->getAgentList(type)){
            SimulationAgent *agent = world->getAgent(type, agentName);
            float radius = 10.0 /((glm::max(rows, cols)*2.0)) /1.1;
            float height = radius*0.7;
            int posX = agent->getInt(currentStep, "x");
            int posY = agent->getInt(currentStep, "y");
            agent->setShape(new Shape(agentSettings.at(type).shape, tiles[posX][posY]->getCenter(), radius, height, Shape::getColor(agentSettings.at(type).colorId), program));

        }
    }
    updateGrid();

}


