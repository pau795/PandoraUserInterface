#include "runningSimulationWindow.h"
#include "ui_runningSimulationWindow.h"

RunningSimulationWindow::RunningSimulationWindow(QString dir, QString executable, int numSimulations, int numSteps, TiXmlDocument* configFile,TiXmlDocument* incFile) :
    QWidget(nullptr),
    ui(new Ui::RunningSimulationWindow)
{
    ui->setupUi(this);
    this->dir = dir;
    this->executable = executable;
    this->numSimulations = numSimulations;
    this->numSteps = numSteps;
    this->configFile = configFile;
    this->initialConfig = new TiXmlDocument(*configFile);
    this->incrementFile = incFile;

    currentStep = 1;
    currentSimulation = 1;
    singleSimulation = numSimulations == 1;
    ui->numSimulationsBar->setRange(0,numSimulations);
    ui->simulationStepsBar->setRange(0, numSteps);
    ui-> okButton->setEnabled(false);

    TiXmlElement* config = configFile->FirstChildElement("config");
    this->initialLogDir = getLogsDir(config);
    this->initialResultFile = getResultFile(config);
    setOutputPaths(currentSimulation);

    setProgressBars();
    connect(this, SIGNAL(newSimulation()), this, SLOT(nextSimulation()));

    emit newSimulation();
}

RunningSimulationWindow::~RunningSimulationWindow()
{
    delete ui;
}


void RunningSimulationWindow::setProgressBars(){

    ui->simulationStepsBar->setValue(currentStep);
    ui->numSimulationsBar->setValue(currentSimulation);
    ui->simulationSteps->setText("Step " + QString::number(currentStep) + "/" + QString::number(numSteps)+".");
    ui->numSimulations->setText("Simulation " +QString::number(currentSimulation) + "/" + QString::number(numSimulations)+".");
}

void RunningSimulationWindow::simulationMessage(QString message){
    ui->consoleText->addLine(message);
    if(message.contains("removeAgents")){
        ++currentStep;
        setProgressBars();
    }
}

void RunningSimulationWindow::nextSimulation(){
    if (currentSimulation <= numSimulations){
        ui->consoleText->addLine("Starting simulation " + QString::number(currentSimulation) +".");
        currentStep = 0;
        setOutputPaths(currentSimulation);
        updateConfigOutput();
        setProgressBars();
        runSimulation();
    }
    else {
        ui->consoleText->addLine("All simulations have finished.");
        this->configFile = initialConfig;
        configFile->SaveFile();

        ui->okButton->setEnabled(true);

    }
}



void RunningSimulationWindow::closeEvent(QCloseEvent* event){
    event->accept();
    ui->consoleText->addLine("Simulation run canceled.");
    simulation->killProcess();
    emit simulationsEnded();
}

void RunningSimulationWindow::simulationFinished(int exitCode){
    ui->consoleText->addLine("Exit code: " + QString::number(exitCode)+".");
    ui->consoleText->addLine("Simulation " + QString::number(currentSimulation) + " finished.");
    int posLastSlash =resultFile.lastIndexOf("/");
    QString configName = dir + resultFile.right(resultFile.length()-1).left(posLastSlash) + "config.xml";
    configFile->SaveFile(configName.toStdString().c_str());
    currentSimulation++;
    if (currentSimulation <= numSimulations) {
        setProgressBars();
        increaseConfigFile(configFile->RootElement(), incrementFile->RootElement());
        QString a = "lmao" + QString::number(currentSimulation);
        TiXmlElement* config = configFile->FirstChildElement("config");
        numSteps = getNumSteps(config);
    }
    emit newSimulation();
}

void RunningSimulationWindow::runSimulation(){
    simulation = new ExecutableRunner(dir, "./"+executable);
    simulation->runProcess();
    connect(simulation, SIGNAL(newMessage(QString)), this, SLOT(simulationMessage(QString)));
    connect(simulation, SIGNAL(processFinished(int)), this, SLOT(simulationFinished(int)));
}



void RunningSimulationWindow::on_okButton_clicked()
{

    emit simulationsEnded();
    this->destroy();
}

QString RunningSimulationWindow::getResultFile(TiXmlElement* root){
    TiXmlElement* node = root->FirstChildElement("output");
    return QString(node->Attribute("resultsFile"));
};

QString RunningSimulationWindow::getLogsDir(TiXmlElement* root){
    TiXmlElement* node = root->FirstChildElement("output");
    return QString(node->Attribute("logsDir"));
};

void RunningSimulationWindow::updateConfigOutput(){
    TiXmlElement* outputNode = configFile->RootElement()->FirstChildElement("output");
    outputNode->SetAttribute("resultsFile", resultFile.toStdString().c_str());
    outputNode->SetAttribute("logsDir", logDir.toStdString().c_str());
    configFile->SaveFile();
}


void  RunningSimulationWindow::setOutputPaths(int i){
    if (numSimulations == 1){
        resultFile = initialResultFile;
        logDir = initialLogDir;
    }
    else {
        int posLastSlash =initialResultFile.lastIndexOf("/");
        resultFile = initialResultFile.left(posLastSlash+1) + "run" + QString::number(i) + initialResultFile.right(initialResultFile.size() - posLastSlash);
        logDir = initialLogDir + "/run"+QString::number(i);
    }
}




void RunningSimulationWindow::on_cancelButton_clicked()
{
    ui->consoleText->addLine("Simulation run canceled.");
    simulation->killProcess();
    emit simulationsEnded();
    this->destroy();
}
