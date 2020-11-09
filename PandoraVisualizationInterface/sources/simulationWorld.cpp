#include "simulationWorld.h"

vector<string> SimulationWorld::agentTypes;
vector<string> SimulationWorld::agentAttributeNames;

SimulationWorld::SimulationWorld(){
}
SimulationWorld::~SimulationWorld(){
    for (map<string, map<string, SimulationAgent*>>::iterator it = agentTypeMap.begin(); it != agentTypeMap.end(); it++ ){
        map<string, SimulationAgent*> &agentMap = it->second;
        for(map<string, SimulationAgent*>::iterator it2 = agentMap.begin(); it2 != agentMap.end(); ++it2){
            delete it2->second;
        }
    }
    agentTypeMap.clear();
    for(map<string, DynamicRaster*>::iterator it = dynamicRasterMap.begin(); it != dynamicRasterMap.end(); ++it){
        delete it->second;
    }
    dynamicRasterMap.clear();
    for(map<string, Raster*>::iterator it = staticRasterMap.begin(); it != staticRasterMap.end(); ++it){
        delete it->second;
    }
    staticRasterMap.clear();
}

unsigned int SimulationWorld::getNumSteps(){
    return numSteps;
}

unsigned int SimulationWorld::getNumTasks(){
    return numTasks;
}

unsigned int SimulationWorld::getWidth(){
    return width;
}

unsigned int SimulationWorld::getHeight(){
    return height;
}

void SimulationWorld::setNumSteps(unsigned int numSteps){
    this->numSteps = numSteps;
}

void SimulationWorld::setNumTasks(unsigned int numTasks){
    this->numTasks = numTasks;
}

void SimulationWorld::setWidth(unsigned int width){
    this->width = width;
}

void SimulationWorld::setHeight(unsigned int height){
    this->height = height;
}

Raster* SimulationWorld::getRaster(string name, int step){
    if (isRasterDynamic(name)){
        DynamicRaster* d = dynamicRasterMap.find(name)->second;
        return d->getRaster(step);
    }
    else return staticRasterMap.find(name)->second;
}

SimulationAgent* SimulationWorld::getAgent(string type, string id){
   map<string, SimulationAgent*> &agentMap = agentTypeMap.at(type);
   return agentMap.at(id);
}

bool SimulationWorld::isRasterDynamic(string name){
    map<string, DynamicRaster*>::iterator it = dynamicRasterMap.find(name);
    if (it == dynamicRasterMap.end()) return false;
    else return true;
}

pair<int, int> SimulationWorld::getRasterMinMaxValues(string rasterName){
    if (isRasterDynamic(rasterName)){
        DynamicRaster* d = dynamicRasterMap.find(rasterName)->second;
        return make_pair(d->getMinValue(), d->getMaxValue());
    }
    else {
        Raster* r = staticRasterMap.find(rasterName)->second;
        return make_pair(r->getMinValue(), r->getMaxValue());
    }
}

void SimulationWorld::addStaticRaster(string name, Raster* raster){
    staticRasterMap.insert(make_pair(name, raster));
}

void SimulationWorld::addDynamicRaster(string name, DynamicRaster* raster){
    dynamicRasterMap.insert(make_pair(name, raster));
}

void SimulationWorld::addAgent(string type, string id, SimulationAgent* agent){
    map<string, map<string, SimulationAgent*>>::iterator it = agentTypeMap.find(type);
    if (it == agentTypeMap.end()){
        map<string, SimulationAgent*> agentMap;
        agentTypeMap.insert(make_pair(type, agentMap));
        it = agentTypeMap.find(type);
    }
    map<string, SimulationAgent*> &agentMap = it->second;
    agentMap.insert(make_pair(id, agent));

}

void SimulationWorld::addAgentType(string type){
    bool found = false;
    for(unsigned int i=0; i < agentTypes.size(); ++i) if(type.compare(agentTypes[i]) == 0) found = true;
    if (not found) agentTypes.push_back(type);
}

vector<string> SimulationWorld::getRasterList(){
    int size = dynamicRasterMap.size() + staticRasterMap.size();
    vector<string> v(size);
    int i = 0;
    for(map<string, DynamicRaster*>::iterator it = dynamicRasterMap.begin(); it != dynamicRasterMap.end(); ++it){
        v[i] = it->first;
        ++i;
    }
    for(map<string, Raster*>::iterator it = staticRasterMap.begin(); it != staticRasterMap.end(); ++it){
        v[i] = it->first;
        ++i;
    }
    return v;
}

vector<string> SimulationWorld::getAgentTypeList(){
    int size = agentTypeMap.size();
    vector<string> v(size);
    int i = 0;
    for (map<string, map<string, SimulationAgent*>>::iterator it = agentTypeMap.begin(); it != agentTypeMap.end(); it++ ){
        v[i] = it->first;
        ++i;
    }
    return v;
}

vector<string> SimulationWorld::getAgentList(string type){

    map<string, SimulationAgent*> &agentMap = agentTypeMap.at(type);
    vector<string> v(agentMap.size());
    int i=0;
    for(map<string, SimulationAgent*>::iterator it = agentMap.begin(); it != agentMap.end(); ++it){
        v[i] = it->first;
        ++i;
    }
    return v;
}


bool SimulationWorld::readFile(string fileName){
    hid_t fileId = H5Fopen(fileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT ); //h5 file open

    if (fileId < 0) { // file not found
            return false;
    }

    //open global, which has global information of the simulation
    hid_t datasetId = H5Dopen(fileId, "global", H5P_DEFAULT);
    int simulationSteps = 0;
    hid_t attributeId = H5Aopen_name(datasetId, "numSteps");  //get the number of steps of the simulation
    H5Aread(attributeId, H5T_NATIVE_INT, &simulationSteps);
    H5Aclose(attributeId);
    int resolution = 0;
    attributeId = H5Aopen_name(datasetId, "serializerResolution");    //get the resolution
    H5Aread(attributeId, H5T_NATIVE_INT, &resolution);
    H5Aclose(attributeId);
    int simulationTasks = 0;
    attributeId = H5Aopen_name(datasetId, "numTasks");    //get the number of tasks
    H5Aread(attributeId, H5T_NATIVE_INT, &simulationTasks);
    H5Aclose(attributeId);
    if (simulationTasks == 0) {    // stop if there are no tasks
        H5Dclose(datasetId);
        H5Fclose(fileId);
        // TODO throw exception
        return false;
    }
    //get the size of the world
    int worldWidth = 0, worldHeight = 0;
    attributeId = H5Aopen_name( datasetId, "width" );
    H5Aread( attributeId, H5T_NATIVE_INT, &worldWidth );
    H5Aclose( attributeId );

    attributeId = H5Aopen_name( datasetId, "height" );
    H5Aread( attributeId, H5T_NATIVE_INT, &worldHeight );
    H5Aclose( attributeId );

    H5Dclose( datasetId ); //close global info dataset

    setNumSteps(simulationSteps);
    setNumTasks(simulationTasks);
    setWidth(worldWidth);
    setHeight(worldHeight);


    //RASTER

    // static rasters
    hid_t staticRasterNamesDatasetId = H5Dopen(fileId, "staticRasters", H5P_DEFAULT);
    int numStaticRasters = H5Aget_num_attrs(staticRasterNamesDatasetId);
    for (int i=0; i<numStaticRasters; i++)
    {
        char nameAttribute[256];
        attributeId= H5Aopen_idx(staticRasterNamesDatasetId, i);
        H5Aget_name(attributeId, 256, nameAttribute);
        QString staticRasterName = nameAttribute;
        QString staticRasterPath = "/" + staticRasterName + "/values";
        H5Aclose(attributeId);
        Raster* raster = readRaster(fileName, staticRasterPath.toStdString());
        this->addStaticRaster(nameAttribute, raster);
    }
    H5Dclose(staticRasterNamesDatasetId);
    // dynamic rasters
    hid_t rasterNamesDatasetId = H5Dopen(fileId, "rasters", H5P_DEFAULT);
    int numDynamicRasters = H5Aget_num_attrs(rasterNamesDatasetId);
    for ( int i=0; i<numDynamicRasters; i++ )
    {
        char nameAttribute[256];
        attributeId= H5Aopen_idx(rasterNamesDatasetId, i);
        H5Aget_name(attributeId, 256, nameAttribute);
        QString dynamicRasterName = nameAttribute;
        QString dynamicRasterPath = "/" + dynamicRasterName + "/step";
        H5Aclose(attributeId);
        DynamicRaster* raster = readDynamicRaster(fileName, dynamicRasterPath.toStdString());
        this->addDynamicRaster(nameAttribute, raster);

    }
    H5Dclose(rasterNamesDatasetId);
    //AGENTS
    unsigned int filePos = fileName.find_last_of("/");
    string path = fileName.substr(0, filePos+1);
    readAgents(path, simulationSteps, numTasks);
    createAgentMap();
    return true;
}


void SimulationWorld::createAgentMap(){
    for (map<string, map<string, SimulationAgent*>>::iterator it = agentTypeMap.begin(); it != agentTypeMap.end(); it++ ){
        map<string, SimulationAgent*> &agentMap = it->second;
        for(map<string, SimulationAgent*>::iterator it2 = agentMap.begin(); it2 != agentMap.end(); ++it2){
            SimulationAgent* agent = it2->second;
            for(unsigned int i=0; i<numSteps; ++i){
                vector<SimulationAgent*> &v = agentsToLoad[i];
                if (not agent->getString(i, "id").empty()) v.push_back(agent);
            }
        }
    }
}

Raster* SimulationWorld::readRaster(string fileName, string rasterPath){
    hid_t fileId = H5Fopen(fileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT); //h5 file open
    hid_t dset_id = H5Dopen(fileId, rasterPath.c_str(), H5P_DEFAULT);
    hid_t dataspaceId = H5Dget_space(dset_id);
    hsize_t dims[2];
    H5Sget_simple_extent_dims(dataspaceId, dims, NULL);  //get raster dimensions
    H5Sclose(dataspaceId);

    int *data = (int*) malloc(sizeof(int)*dims[0]*dims[1]); //pointer to the raster map
    H5Dread(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
    H5Dclose(dset_id);
    Raster* raster = new Raster();
    raster->setSize(dims[0],dims[1]);
    size_t index = 0;
    for (size_t y=0; y<dims[0]; y++){
        for (size_t x=0; x<dims[1]; x++) {
            int value = data[index];
            if (value > raster->getMaxValue())raster->setMaxValue(value);
            if (value < raster->getMinValue()) raster->setMinValue(value);
            raster->setCell(x, y, data[index]);
            ++index;
        }
    }
    free(data);
    H5Fclose(fileId);
    return raster;
}

DynamicRaster* SimulationWorld::readDynamicRaster(string fileName, string rasterPath){
    DynamicRaster* raster = new DynamicRaster(numSteps);
    raster->setSize(this->getWidth(), this->getHeight());
    for (unsigned int i = 0; i < raster->getNumSteps(); ++i){
        QString path = QString::fromStdString(rasterPath) + QString::number(i);
        Raster* r = readRaster(fileName, path.toStdString());
        if (r->getMinValue() < raster->getMinValue()) raster->setMinValue(r->getMinValue());
        if (r->getMaxValue() > raster->getMaxValue()) raster->setMaxValue(r->getMaxValue());
        raster->setRaster(i, r);
    }
    return raster;

}

herr_t SimulationWorld::iterateAgentTypes(hid_t loc_id, const char * name, const H5L_info_t *linfo, void *opdata)
{
    agentTypes.push_back(name);
    return 0;
}

herr_t SimulationWorld::iterateAgentAttributes( hid_t loc_id, const char * name, const H5L_info_t *linfo, void *opdata ){
    agentAttributeNames.push_back(name);
    return 0;
}

hssize_t SimulationWorld::registerAgentIds(const hid_t &stepGroup, vector<string> &indexAgents, string agentType){
    hid_t datasetId = H5Dopen(stepGroup, "id", H5P_DEFAULT);
    hid_t stringType = H5Tcopy (H5T_C_S1);
    H5Tset_size(stringType, H5T_VARIABLE);

    hid_t stringSpace = H5Dget_space(datasetId);
    hssize_t numElements = H5Sget_simple_extent_npoints(stringSpace); // get the number of elements
    if (numElements==0) {
        H5Sclose(stringSpace);
        H5Tclose(stringType);
        H5Dclose(datasetId);
        return numElements;
    }
    char ** stringIds = (char **) malloc(numElements *sizeof(char *));
    H5Dread(datasetId, stringType, H5S_ALL, H5S_ALL, H5P_DEFAULT, stringIds);

    for (int i=0; i < numElements; i++){
        string agentName(stringIds[i]);
        indexAgents.push_back(agentName);
        this->addAgent(agentType, agentName, new SimulationAgent(agentType, numSteps));
    }
    // clean memory
    H5Dvlen_reclaim(stringType, stringSpace, H5P_DEFAULT, stringIds);
    free (stringIds );
    H5Sclose(stringSpace);
    H5Tclose(stringType);
    H5Dclose(datasetId);
    return numElements;


}

vector<string> SimulationWorld::getAgentTypes(){
    return agentTypes;
}

vector<SimulationAgent*> SimulationWorld::getAgentsToLoad(int step){
    if (agentTypeMap.size() > 0) return agentsToLoad.at(step);
    else return vector<SimulationAgent*>();

}

void SimulationWorld::readAgents(string path, unsigned int numSteps, unsigned int numTasks){
    for (unsigned int i = 0; i < numTasks; i++) {
        QString fileName = QString::fromStdString(path) +"agents-" + QString::number(i) + ".abm";
        hid_t agentsFileId = H5Fopen(fileName.toStdString().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
        hid_t rootGroup = H5Gopen(agentsFileId, "/", H5P_DEFAULT);
        //iterates through the dataset and stores the name of the agent types in the agentTypes vector.
        H5Literate(rootGroup, H5_INDEX_NAME, H5_ITER_INC, 0, iterateAgentTypes, 0 );

        for (unsigned int t=0; t < agentTypes.size(); ++t){

            string type = agentTypes[t];
            for (currentStep = 0; currentStep < numSteps; ++currentStep){

                QString agentPath = "/" + QString::fromStdString(type) +"/step" + QString::number(currentStep);
                hid_t stepGroup = H5Gopen(agentsFileId, agentPath.toStdString().c_str(), H5P_DEFAULT); //current step index for the current agent type

                //iterates through the current step, and gets all the name attributes of the agent type, storing the content in the agentAttributesNames vector.
                H5Literate(stepGroup, H5_INDEX_NAME, H5_ITER_INC, 0, iterateAgentAttributes, 0);
                vector<string> indexAgents;
                hssize_t numElements = registerAgentIds(stepGroup, indexAgents, type);
                if (numElements!=0) loadAttributes(stepGroup, numElements, indexAgents, type);
                H5Gclose( stepGroup );
                agentAttributeNames.clear( ); // clearing the attribute names for the next iteration
            }
        }
        H5Gclose( rootGroup );
        H5Fclose( agentsFileId );
        agentTypes.clear( ); // clearing the types for the next iteration
    }
}

void SimulationWorld::loadAttributes(const hid_t &stepGroup, hssize_t &numElements, const vector<string> &indexAgents, string agentType){
    for(string attribute:agentAttributeNames){
        hid_t attributeDatasetId = H5Dopen(stepGroup, (attribute).c_str(), H5P_DEFAULT);
        hid_t typeAttribute = H5Dget_type(attributeDatasetId);
        H5T_class_t typeClass = H5Tget_class(typeAttribute);
        if (typeClass == H5T_INTEGER){
            vector<int> data(numElements);
            H5Dread(attributeDatasetId, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(data.at(0)));
            for (int i=0; i < numElements; i++) {
                string agentName = indexAgents.at(i);
                SimulationAgent* agent = getAgent(agentType, agentName);
                agent->addInt(currentStep, attribute, data[i]);
            }
        }
        else if (typeClass == H5T_STRING){
            hid_t stringType = H5Tcopy (H5T_C_S1);
            H5Tset_size( stringType, H5T_VARIABLE );
            char ** stringIds = (char **)malloc (numElements *sizeof(char*));
            H5Dread( attributeDatasetId, stringType, H5S_ALL, H5S_ALL, H5P_DEFAULT, stringIds );
            for (int i = 0; i < numElements; i++) {
                string agentName = indexAgents.at(i);
                SimulationAgent* agent = getAgent(agentType, agentName);
                agent->addString(currentStep, attribute, string(stringIds[i]));
            }
        }
        else if (typeClass == H5T_FLOAT){
            vector<float> data(numElements);
            H5Dread(attributeDatasetId, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(data.at(0)));
            for (int i=0; i < numElements; i++) {
                string agentName = indexAgents.at(i);
                SimulationAgent* agent = getAgent(agentType, agentName);
                agent->addFloat(currentStep, attribute, data[i]);
            }
        }
        else throw 1;
        H5Tclose(typeAttribute);
        H5Dclose(attributeDatasetId);
    }
}

void SimulationWorld::writeStuff(){
    for (map<string, map<string, SimulationAgent*>>::iterator it = agentTypeMap.begin(); it != agentTypeMap.end(); it++ ){
        cout << it->first << endl;
        map<string, SimulationAgent*> &agentMap = it->second;
        for(map<string, SimulationAgent*>::iterator it2 = agentMap.begin(); it2 != agentMap.end(); ++it2){
            cout << "   "<<it2->first << endl;
            SimulationAgent* agent = it2->second;
            for(unsigned int i=0; i<numSteps; ++i){
                cout << agent->getInt(i, "x") << " " << agent->getInt(i, "y") << endl;
            }
        }
    }
    cout << "DYNAMIC RASTER" << endl;
    for(map<string, DynamicRaster*>::iterator it = dynamicRasterMap.begin(); it != dynamicRasterMap.end(); ++it){
        cout << "   "<<it->first << endl;
        DynamicRaster* raster = it->second;
        int s = raster->getNumSteps();
        for (int i=0; i<s; ++i){
            Raster* r = raster->getRaster(i);
            int w = r->getWidth();
            int h = r->getHeight();
            for (int j = 0; j < w; ++j){
                for (int k = 0; k < h; ++k){
                    int value = r->getCell(j, k);
                    cout << value;
                }
                cout << endl;
            }
            cout << endl;
        }
    }
    cout << "STATIC RASTER" << endl;
    for(map<string, Raster*>::iterator it = staticRasterMap.begin(); it != staticRasterMap.end(); ++it){
        cout << "   "<<it->first << endl;
        Raster* r = it->second;

        int w = r->getWidth();
        int h = r->getHeight();
        for (int j = 0; j < w; ++j){
            for (int k = 0; k < h; ++k){
                int value = r->getCell(j, k);
                cout << value;
            }
            cout << endl;
        }
        cout << endl;
    }


}


