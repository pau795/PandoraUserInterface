#include "mainWindow.h"
#include "ui_mainWindow.h"



MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    modelPath = "../examples";
    parameterContainer =  ui->parameterContainer;
    QGroupBox* consoleBox = ui->consoleGroupBox;
    consoleBox->setVisible(false);
    runModelAfterCompile = false;
    incrementVisible = false;
    ui->numSimulations->setValidator(new QIntValidator());
    connect(this, SIGNAL(readyToRunModel(QString, QString)), this, SLOT(runModel(QString, QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
           delete item->widget();
        }
        delete item;
    }
}

void MainWindow::on_BrowserButton_clicked()
{
    ui->consoleText->addLine("Searching Model...");
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Load Model"), modelPath, tr("XML Files (*.xml)"));
    if (!filePath.isEmpty()) {
        ui->consoleText->addLine("Loading Model...");
        modelPath = filePath;
        ui->pathLine->setText(modelPath);
        configFile = new TiXmlDocument();
        *configFile = loadConfig(filePath.toStdString());
        incrementFile = new TiXmlDocument(*configFile);
        setToZero(incrementFile->RootElement());
        TiXmlElement* config = configFile->FirstChildElement("config");
        clearLayout(parameterContainer->layout());
        loadParameters(parameterContainer, config);
        ui->saveButton->setEnabled(true);
        ui->runButton->setEnabled(true);
        ui->actionRun_Model->setEnabled(true);
        ui->actionCompile_Model->setEnabled(true);
        ui->singleRunRButton->setEnabled(true);
        ui->multipleRunsRButton->setEnabled(true);
        ui->singleRunRButton->clicked(true);
        ui->singleRunRButton->setChecked(true);
        ui->consoleText->addLine("Model "+modelPath +" loaded correctly.");

    }
    else ui->consoleText->addLine("Model not loaded.");

}

void MainWindow::on_saveButton_clicked()
{
    writeChanges(configFile, parameterContainer);
    writeIncrement(incrementFile, parameterContainer);
    configFile->SaveFile();
    ui->consoleText->addLine("Model Saved.");
}

void MainWindow::on_runButton_clicked()
{
    on_saveButton_clicked();
    TiXmlElement* config = configFile->FirstChildElement("config");
    simulationSteps = getNumSteps(config);
    numSimulations = std::atoi(ui->numSimulations->text().toStdString().c_str());
    modelDir = getDirPath(modelPath);
    modelExecutable = findExecutable(modelDir);
    if(modelExecutable == "executableError")ui->consoleText->addLine("Could not find the executable name file. Verify that the SConstruct file is correct.");
    else if(modelExecutable == "sconsError") ui->consoleText->addLine("SConstruct file was not found!");
    else {
        QString executablePath = modelDir +"/" + modelExecutable;
        QCheckBox* cbox = ui->compileCheckBox;
        if (cbox->isChecked() or not fileExists(executablePath)) {
            if (not fileExists(executablePath)) ui->consoleText->addLine("Executable file was not found!");
            ui->consoleText->addLine("Proceding to compile the model...");
            runModelAfterCompile = true;
            compileModel(modelDir);

        }
        else emit readyToRunModel(modelDir, modelExecutable);
    }
}

void MainWindow::on_consoleCheckBox_clicked(bool checked)
{
    ui->actionShow_Console->setChecked(checked);
    QGroupBox* consoleBox = ui->consoleGroupBox;
    consoleBox->setVisible(checked);
}

void MainWindow::on_actionSave_Model_triggered()
{
    on_saveButton_clicked();
}

void MainWindow::on_actionCompile_Model_triggered()
{
    modelDir = getDirPath(modelPath);
    modelExecutable = findExecutable(modelDir);
    if(modelExecutable == "executableError")ui->consoleText->addLine("Could not find the executable name file. Verify that the SConstruct file is correct.");
    else if(modelExecutable == "sconsError") ui->consoleText->addLine("SConstruct file was not found!");
    else {
        ui->consoleText->addLine("Proceding to compile the model...");
        runModelAfterCompile=false;
        compileModel(modelDir);
    }
}

void MainWindow::on_actionRun_Model_triggered()
{
    on_runButton_clicked();
}


void MainWindow::on_actionClear_Console_triggered()
{
    ui->consoleText->setText("");

}

void MainWindow::on_actionOpen_Model_triggered()
{
    on_BrowserButton_clicked();
}


void MainWindow::on_compileCheckBox_stateChanged(int arg1)
{
    bool state = arg1 != 0;
    ui->actionCompile_Model_before_running->setChecked(state);
}

void MainWindow::on_actionCompile_Model_before_running_triggered(bool checked)
{
    ui->compileCheckBox->setChecked(checked);
}



void MainWindow::on_actionShow_Console_triggered(bool checked)
{
    ui->consoleCheckBox->setChecked(checked);
    on_consoleCheckBox_clicked(checked);
}

void MainWindow::executableMessage(QString message){
    ui->consoleText->addLine(message.left(message.size()-1));
}

void MainWindow::compilationFinished(int exitCode){
    if (exitCode == 0){
        ui->consoleText->addLine("Model"+ modelExecutable + " was compiled succesfully.");
        if (runModelAfterCompile) emit readyToRunModel(modelDir, modelExecutable);
    }
    else {
        ui->consoleText->addLine("Model "+ modelExecutable + " could not be compiled.");
    }

}

void MainWindow::compileModel(QString dir){
    compiler = new ExecutableRunner(dir, "scons");
    compiler->runProcess();
    QObject::connect(compiler, SIGNAL(newMessage(QString)), this, SLOT(executableMessage(QString)));
    QObject::connect(compiler, SIGNAL(processFinished(int)), this, SLOT(compilationFinished(int)));

}

void MainWindow::displayIncrement(bool show){
    showIncrementWidget(parameterContainer, show);
}

void MainWindow::activateWindow(){
    this->setEnabled(true);
}

void MainWindow::runModel(QString dir, QString executable){
    ui->consoleText->addLine("Proceding to run the " + executable + "model.");
    RunningSimulationWindow* runWindow = new RunningSimulationWindow(dir, executable, numSimulations, simulationSteps, configFile, incrementFile);
    runWindow->show();
    connect(runWindow, SIGNAL(simulationsEnded()), this, SLOT(activateWindow()));
    this->setDisabled(true);
}


void MainWindow::on_singleRunRButton_clicked()
{
    incrementVisible = false;
    ui->numSimulations->setText("1");
    displayIncrement(incrementVisible);
}

void MainWindow::on_multipleRunsRButton_clicked()
{
    incrementVisible = true;
    displayIncrement(incrementVisible);
}

