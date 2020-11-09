#include "simulationAgent.h"

SimulationAgent::SimulationAgent(string type, int numSteps){
    this->numSteps = numSteps;
    this->agentType = type;
    shape = NULL;
}

SimulationAgent::~SimulationAgent(){
    if (shape != NULL) delete shape;
}

void SimulationAgent::addInt(int step, string &key, int value){
    map<string, vector<int>>::iterator it = intAttributes.find(key);
    if ( it == intAttributes.end()){
        vector<int> values(numSteps);
        intAttributes.insert(make_pair(key, values));
        it = intAttributes.find(key); //get iterator to the new vector add the item afterwards
    }
    vector<int> &vector = it->second;
    vector[step] = value;
}

void SimulationAgent::addFloat(int step, string &key, float value){
    map<string, vector<float>>::iterator it = floatAttributes.find(key);
    if ( it == floatAttributes.end()){
        vector<float> values(numSteps);
        floatAttributes.insert(make_pair(key, values));
        it = floatAttributes.find(key); //get iterator to the new vector to add the item afterwards
    }
    vector<float> &vector = it->second;
    vector[step] = value;
}

void SimulationAgent::addString(int step, string &key, const string &value){
    map<string, vector<string>>::iterator it = stringAttributes.find(key);
    if ( it == stringAttributes.end()){
        vector<string> values(numSteps);
        stringAttributes.insert(make_pair(key, values));
        it = stringAttributes.find(key); //get iterator to add the item afterwards
    }
    vector<string> &vector = it->second;
    vector[step] = value;
}

int SimulationAgent::getInt(int step, const string & key){
    map<string, vector<int>>::iterator it = intAttributes.find(key);
    if (it == intAttributes.end()) throw 1;
    const vector<int> & vector = it->second;
    return vector.at(step);
}

float SimulationAgent::getFloat(int step,const string & key){
    map<string, vector<float>>::iterator it = floatAttributes.find(key);
    if (it == floatAttributes.end()) throw 1;
    const vector<float> &vector = it->second;
    return vector.at(step);
}

string &SimulationAgent::getString(int step, const string & key){
    map<string, vector<string>>::iterator it = stringAttributes.find(key);
    if (it == stringAttributes.end()) throw 1;
    vector<string> &vector = it->second;
    return vector.at(step);
}

string SimulationAgent::getAgentType(){
    return agentType;
}


vector<string> SimulationAgent::getAttributeList(){
    int size = intAttributes.size() + floatAttributes.size() + stringAttributes.size();
    vector<string>v(size);
    int i = 0;
    for(map<string,vector<int>>::iterator it = intAttributes.begin(); it != intAttributes.end(); ++it){
        v[i] = it->first;
        ++i;
    }
    for(map<string,vector<float>>::iterator it = floatAttributes.begin(); it != floatAttributes.end(); ++it){
        v[i] = it->first;
        ++i;
    }
    for(map<string,vector<string>>::iterator it = stringAttributes.begin(); it != stringAttributes.end(); ++it){
        v[i] = it->first;
        ++i;
    }
    return v;
}

Shape* SimulationAgent::getShape(){
    return shape;
}
void SimulationAgent::setShape(Shape* shape){
    if (this->shape != NULL) delete this->shape;
    this->shape = shape;
}

bool SimulationAgent::isInt(string & key ){
    map<string, vector<int>>::iterator it = intAttributes.find(key);
    if (it == intAttributes.end()) return false;
    else return true;
}

bool SimulationAgent::isFloat(string & key ){
    map<string, vector<float>>::iterator it = floatAttributes.find(key);
    if (it == floatAttributes.end()) return false;
    else return true;
}

bool SimulationAgent::isString(string & key ){
    map<string, vector<string>>::iterator it = stringAttributes.find(key);
    if (it == stringAttributes.end()) return false;
    else return true;
}

void SimulationAgent::setNumSteps(int numSteps){
    this->numSteps = numSteps;
}
int SimulationAgent::getNumSteps(){
    return numSteps;
}
