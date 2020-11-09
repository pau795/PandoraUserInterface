#ifndef SIMULATIONWORLD_H
#define SIMULATIONWORLD_H

#include <map>
#include <raster.h>
#include <iostream>
#include <dynamicRaster.h>
#include <simulationAgent.h>
#include <unordered_map>
#include <QString>
#include <hdf5.h>

using namespace std;

class SimulationWorld
{
public:
    SimulationWorld();
    ~SimulationWorld();

    unsigned int getNumSteps();
    unsigned int getNumTasks();
    unsigned int getWidth();
    unsigned int getHeight();

    void setNumSteps(unsigned int numSteps);
    void setNumTasks(unsigned int numTasks);
    void setWidth(unsigned int width);
    void setHeight(unsigned int height);

    Raster* getRaster(string name, int step);
    SimulationAgent* getAgent(string type, string id);
    vector <string> getRasterList();
    vector <string> getAgentTypeList();
    vector <string> getAgentList(string type);
    static vector<string> getAgentTypes();
    bool isRasterDynamic(string name);
    pair<int, int> getRasterMinMaxValues(string rasterName);

    void addStaticRaster(string name, Raster* raster);
    void addDynamicRaster(string name, DynamicRaster* raster);
    void addAgent(string type, string id, SimulationAgent* agent);
    static void addAgentType(string type);

    vector<SimulationAgent*> getAgentsToLoad(int step);

    bool readFile(string fileName);
    void writeStuff();




private:
    unsigned int width, height;
    unsigned int numTasks, numSteps, currentStep;
    map<string, Raster*> staticRasterMap;
    map<string, DynamicRaster*> dynamicRasterMap;
    map<string, map<string, SimulationAgent*>> agentTypeMap;
    static vector<string> agentTypes;
    static vector<string> agentAttributeNames;

    unordered_map<int, vector<SimulationAgent*>> agentsToLoad;

    void createAgentMap();

    Raster* readRaster(string fileName, string rasterPath);
    DynamicRaster* readDynamicRaster(string fileName, string rasterPath);
    void readAgents(string path, unsigned int numSteps, unsigned int numTasks);
    static herr_t iterateAgentTypes(hid_t loc_id, const char * name, const H5L_info_t *linfo, void *opdata);
    static herr_t iterateAgentAttributes(hid_t loc_id, const char * name, const H5L_info_t *linfo, void *opdata);
    hssize_t registerAgentIds(const hid_t & stepGroup, vector<string> &indexAgents, string agentType);
    void loadAttributes(const hid_t & stepGroup, hssize_t &numElements, const vector<string> &indexAgents, string agentType);
};

#endif // SIMULATIONWORLD_H
