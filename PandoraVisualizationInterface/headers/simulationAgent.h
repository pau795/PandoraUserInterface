#ifndef SIMULATIONAGENT_H
#define SIMULATIONAGENT_H

#include <string>
#include <vector>
#include <map>
#include <shape.h>

using namespace std;
class SimulationAgent
{
public:
    SimulationAgent(string type, int numSteps);
    ~SimulationAgent();

    void addInt( int numStep, string &key, int value);
    void addFloat( int numStep, string &key, float value);
    void addString( int numStep, string &key, const string &value);

    int getInt(int step, const string &key);
    float getFloat(int step, const string &key);
    string &getString(int step, const string &key);
    vector<string> getAttributeList();
    string getAgentType();
    Shape* getShape();
    void setShape(Shape* shape);

    bool isInt(string &key);
    bool isFloat(string &key);
    bool isString(string &key);

    void setNumSteps(int numSteps);
    int getNumSteps();

private:

    int numSteps;
    map<string, vector<int>> intAttributes;
    map<string, vector<float>> floatAttributes;
    map<string, vector<string>> stringAttributes;

    string agentType;
    Shape *shape;


};

#endif // SIMULATIONAGENT_H
