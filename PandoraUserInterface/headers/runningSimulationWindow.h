#ifndef RUNNINGSIMULATIONWINDOW_H
#define RUNNINGSIMULATIONWINDOW_H

#include <QWidget>
#include <iostream>
#include "executableRunner.h"
#include "QCloseEvent"
#include <QFileSystemWatcher>
#include "configLoader.h"
#include "tinyxml.h"
#include "utils.h"

namespace Ui {
class RunningSimulationWindow;
}

class RunningSimulationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RunningSimulationWindow(QString dir, QString executable, int numSimulations, int numSteps, TiXmlDocument* configFile, TiXmlDocument* incFile);
    ~RunningSimulationWindow();

signals:
    void newSimulation();
    void simulationsEnded();

public slots:
    virtual void closeEvent(QCloseEvent* event) override;
    void nextSimulation();
    void simulationMessage(QString message);
    void simulationFinished(int exitCode);

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();


private:
    Ui::RunningSimulationWindow *ui;
    QString dir;
    QString executable;
    QString resultFile, initialResultFile;
    QString logDir, initialLogDir;
    ExecutableRunner* simulation;
    TiXmlDocument* configFile;
    TiXmlDocument* incrementFile;
    TiXmlDocument* initialConfig;

    int numSimulations;
    int numSteps;
    int currentSimulation;
    int currentStep;
    int logPointer;
    bool singleSimulation;

    void setProgressBars();
    void runSimulation();
    void updateConfigOutput();

    QString getResultFile(TiXmlElement* root);
    QString getLogsDir(TiXmlElement* root);



    void setOutputPaths(int i); // set the result file and log dir paths for the i simulation





};

#endif // RUNNINGSIMULATIONWINDOW_H
