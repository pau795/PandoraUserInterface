#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gl = ui->visualizer;
    numAgents = 50;
    numSteps = 150;
    rows =100;
    cols = 100;
    world = NULL;
    stepsPerSecond = 3;
    stepInc = 1;
    modelPath = "../examples";
    ui->sliderPanel->setEnabled(false);
    ui->settingsGroup->setEnabled(false);
    this->timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
    setContextMenuPolicy(Qt::NoContextMenu);
    this->play = false;
    ui->actionReset_Simulation->setEnabled(false);
    ui->actionSet_timer->setEnabled(false);
    ui->actionForward->setEnabled(false);
    ui->actionRewind->setEnabled(false);
    ui->actionPlay->setEnabled(false);
    ui->actionReset_View->setEnabled(false);
    ui->actionTerrain_Elevation->setEnabled(false);
    agentTypeMenu =  new QMenu(ui->agentView);
    agentTypeMenu->addAction(ui->actionSet_agent_type_figure);
    rasterModel = new QStandardItemModel(0, 0);
    rasterModel->setHorizontalHeaderItem( 0, new QStandardItem("Rasters"));
    agentModel = new QStandardItemModel(0, 0);
    agentModel->setHorizontalHeaderItem( 0, new QStandardItem("Agents"));
    ui->rasterView->setModel(rasterModel);
    ui->agentView->setModel(agentModel);

}

MainWindow::~MainWindow()
{
    delete ui;
    if (world != NULL) delete world;
    deleteRasterModel();
    deleteAgentModel();

}

void MainWindow::deleteRasterModel(){
    for(int i=0; i< rasterModel->rowCount(); ++i){
            QStandardItem* item = rasterModel->item(i,0);
            delete item;
        }
    delete rasterModel;
    rasterSettings.clear();
}

void MainWindow::deleteAgentModel(){
    for(int i=0; i< agentModel->rowCount(); ++i){
            QStandardItem* item = agentModel->item(i,0);
            for (int k=0; k < item->rowCount(); ++k){
                QStandardItem* child = item->child(k);
                for(int l=0; l< child->rowCount(); ++l){
                    QStandardItem* attrib0 = child->child(l, 0);
                    QStandardItem* attrib1 = child->child(l, 1);
                    delete attrib0;
                    delete attrib1;
                }
                delete child;
            }
            delete item;
    }
    delete agentModel;
    agentSettings.clear();
}

void MainWindow::on_actionTerrain_Elevation_triggered(){
    gl->setElevation();
    if (ui->actionTerrain_Elevation->isChecked()) ui->elevationLabel->setText("Elevation Enabled");
    else  ui->elevationLabel->setText("Elevation Disabled");
}

void MainWindow::on_actionReset_View_triggered(){
    gl->resetView();
}

void MainWindow::on_actionReset_Simulation_triggered(){
    stepsPerSecond = 3;
    stepInc = 1;
    ui->speedValue->setText(QString::number(stepsPerSecond*abs(stepInc))+" steps per second");
    ui->stepSlider->setValue(1);
    ui->actionPlay->setIcon(QIcon("icons/play.png"));
    timer->stop();
    ui->stepSlider->setFocus();
}

void MainWindow::on_actionPlay_triggered(){
    if (play) {
        ui->actionPlay->setIcon(QIcon("icons/pause.png"));
        timer->start(1000/abs(stepsPerSecond));
        ui->visualizer->setFocus();
    }
    else {
       ui->actionPlay->setIcon(QIcon("icons/play.png"));
       timer->stop();
       ui->stepSlider->setFocus();
    }
    this->play = !play;
}


void MainWindow::on_actionOpen_Simulation_triggered(){
    loadSimulation();
}

void MainWindow::on_agentView_expanded(const QModelIndex &index){
    updateAgentsValue();
}

void MainWindow::on_actionForward_triggered(){
    ++stepsPerSecond;
    if (stepsPerSecond == 0) stepsPerSecond = 1;
    if (stepsPerSecond > 0 and stepInc < 0) stepInc = -stepInc;
    if (not play){
        play = true;
        timer->stop();
        on_actionPlay_triggered();
    }
    ui->speedValue->setText(QString::number(stepsPerSecond*abs(stepInc))+" steps per second");

}

void MainWindow::on_actionRewind_triggered(){
    --stepsPerSecond;
    if (stepsPerSecond == 0) stepsPerSecond = -1;
    if (stepsPerSecond < 0 and stepInc > 0) stepInc = -stepInc;
    if (not play){
        play = true;
        timer->stop();
        on_actionPlay_triggered();
    }
    ui->speedValue->setText(QString::number(stepsPerSecond*abs(stepInc))+" steps per second");
}

void MainWindow::on_actionSet_timer_triggered(){

}

void MainWindow::on_actionSet_agent_type_figure_triggered(){
    Shape::ShapeSettings &st = agentSettings.at(agentModel->itemFromIndex(agentTypeIndex)->text().toStdString());
    AgentSettingsDialog *dialog = new AgentSettingsDialog(st, this);
    QObject::connect(dialog, SIGNAL(settingsChanged(Shape::ShapeSettings &)), this, SLOT(agentSettingsChanged(Shape::ShapeSettings &)));
    dialog->show();
}


void MainWindow::timerTick(){
   if (cStep + stepInc < numSteps && cStep + stepInc > 1) ui->stepSlider->setValue(cStep+stepInc);
   else {
       if (cStep + stepInc >= numSteps) ui->stepSlider->setValue(numSteps);
       else if (cStep + stepInc <= 1) ui->stepSlider->setValue(1);
       timer->stop();
       ui->actionPlay->setIcon(QIcon("icons/play.png"));
       this->play = true;
       ui->stepSlider->setFocus();
   }
}



void MainWindow::setupSimulationUI(){
    this->numSteps = world->getNumSteps();
    this->cStep = 1;
    this->rows = world->getHeight();
    this->cols = world->getWidth();
    ui->step0->setText("Step 1");
    ui->currentStep->setRange(1, numSteps);
    ui->currentStep->setValue(1);
    ui->maxStep->setText("Step " + QString::number(numSteps));
    ui->sliderPanel->setEnabled(true);
    ui->settingsGroup->setEnabled(true);
    ui->stepSlider->setRange(1, numSteps);
    QObject::connect(ui->stepSlider, SIGNAL(valueChanged(int)), ui->currentStep, SLOT(setValue(int)));
    QObject::connect(ui->currentStep, SIGNAL(valueChanged(int)), ui->stepSlider, SLOT(setValue(int)));
    QObject::connect(ui->currentStep, SIGNAL(valueChanged(int)), this , SLOT(setCurrentStep(int)));
    setRasterView();
    setAgentView();
    ui->agentView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->agentView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    ui->actionReset_Simulation->setEnabled(true);
    ui->actionPlay->setEnabled(true);
    ui->actionPlay->setIcon(QIcon("icons/play.png"));
    this->play = true;
    ui->actionReset_View->setEnabled(true);
    ui->actionTerrain_Elevation->setEnabled(true);
    ui->actionSet_timer->setEnabled(true);
    ui->actionForward->setEnabled(true);
    ui->actionRewind->setEnabled(true);
    ui->stepSlider->setFocus();
    gl->setSimulationParameters(world, getRasterToDraw(), agentSettings);
    gl->setEnabled(true);

}

void MainWindow::onCustomContextMenu(const QPoint &point){
    QModelIndex index = ui->agentView->indexAt(point);
        if (index.isValid() && agentModel->itemFromIndex(index)->isCheckable()) {
            agentTypeIndex = index;
            agentTypeMenu->exec(ui->agentView->viewport()->mapToGlobal(point));
        }
}



void MainWindow::setCurrentStep(int step){
    if (step != cStep){
        this->cStep = step;
        updateAgentsValue();
        gl->setCurrentStep(cStep-1);
    }
}

void MainWindow::loadSimulation(){
    ui->actionTerrain_Elevation->setChecked(true);
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Load Simulation"), modelPath, tr("H5 Files (*.h5)"));
    if (!filePath.isEmpty()) {
        modelPath = filePath;
        ui->simulationPath->setText(filePath);
        if (world != NULL) delete world;
        world = new SimulationWorld();
        bool b = world->readFile(filePath.toStdString());
        if (b){
            setupSimulationUI();
        }
        else cout << "Simulation couldn't be loadded." <<endl;
    }
}

void MainWindow::setRasterView(){
    deleteRasterModel();
    vector<string> rasterNames = world->getRasterList();
    rasterModel = new QStandardItemModel(rasterNames.size(), 1);
    for (int i=0; i< rasterNames.size(); ++i){
        QString raster = QString::fromStdString(rasterNames[i]);
        rasterSettings.insert(make_pair(raster.toStdString(), true));
        QStandardItem *item = new QStandardItem(raster);
        item->setCheckable(true);
        item->setEditable(false);
        item->setCheckState(Qt::Checked);
        rasterModel->setItem(i,0, item);
    }
    rasterModel->setHorizontalHeaderItem( 0, new QStandardItem("Rasters"));
    QObject::connect(rasterModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(rasterItemChange(QStandardItem*)));
    ui->rasterView->setModel(rasterModel);

}

void MainWindow::agentItemChange(QStandardItem* item){
    if (item->isCheckable()){
        agentSettings[item->text().toStdString()].active = item->checkState();
    }
    gl->setAgentSettings(agentSettings);

}

void MainWindow::rasterItemChange(QStandardItem* item){
    if (item->isCheckable()){
        rasterSettings[item->text().toStdString()] = item->checkState();
    }
    gl->setCurrentRaster(getRasterToDraw());

}

void MainWindow::selectedAgentItem(const QItemSelection& selected, const QItemSelection& deselected){

}

void MainWindow::agentSettingsChanged(Shape::ShapeSettings &settings){
    QStandardItem *item = agentModel->itemFromIndex(agentTypeIndex);
    cout << settings.shape << " " << settings.colorId << endl;;
    agentSettings[item->text().toStdString()] = settings;
    QIcon icon(QString::fromStdString(Shape::getIconName(settings.shape)));
    item->setIcon(icon);
    gl->setAgentSettings(agentSettings);
}

QString MainWindow::getAgentValue(SimulationAgent *agent, string attribute){
    QString value;
    if (agent->isInt(attribute)){
        int v = agent->getInt(cStep-1, attribute);
        value = QString::number(v);
    }
    else if (agent->isFloat(attribute)){
        float v = agent->getFloat(cStep-1, attribute);
        value = QString::number(v);
    }
    else if (agent->isString(attribute)){
        string v = agent->getString(cStep-1, attribute);
        value = QString::fromStdString(v);
    }
    return value;
}

void MainWindow::updateAgentsValue(){
    for(int i=0; i< agentModel->rowCount(); ++i){
            QStandardItem* item = agentModel->item(i,0);
            if (ui->agentView->isExpanded(item->index())){
                QString agentType = item->text();
                for (int k=0; k < item->rowCount(); ++k){
                    QStandardItem* child = item->child(k);
                    if (ui->agentView->isExpanded(child->index())){
                        QString agentName = child->text();
                        for(int l=0; l< child->rowCount(); ++l){
                            QStandardItem* attrib = child->child(l, 0);
                            QStandardItem* attribValue = child->child(l, 1);
                            QString attribName = attrib->text();
                            SimulationAgent* agent = world->getAgent(agentType.toStdString(), agentName.toStdString());
                            QString value = getAgentValue(agent, attribName.toStdString());
                            attribValue->setText(value);
                        }
                    }
                }
            }
    }
}

string MainWindow::getRasterToDraw(){
    vector<string> rasterList = world->getRasterList();
    for (int i=0; i < rasterList.size(); ++i){
        if(rasterSettings[rasterList[i]]) return rasterList[i];
    }
}

void MainWindow::setAgentView(){
    deleteAgentModel();
    vector<string> agentTypes = world->getAgentTypeList();
    agentModel = new QStandardItemModel(agentTypes.size(), 2);
    for (int i=0; i< agentTypes.size(); ++i){
        QString type = QString::fromStdString(agentTypes[i]);
        Shape::ShapeSettings st;
        st.active = true;
        st.colorId = Shape::getDefaultColorId(i);
        st.shape = Shape::getDefaultShape(i);
        agentSettings.insert(make_pair(type.toStdString(), st));
        QStandardItem *item = new QStandardItem(type);
        QIcon icon(QString::fromStdString(Shape::getIconName(st.shape)));
        item->setIcon(icon);
        item->setCheckable(true);
        item->setEditable(false);
        item->setCheckState(Qt::Checked);
        vector<string> agentNames = world->getAgentList(type.toStdString());
        for (int j=0; j < agentNames.size(); ++j){
            QString agentName = QString::fromStdString(agentNames[j]);
            QStandardItem *child = new QStandardItem(agentName);
            child->setEditable(false);
            SimulationAgent* agent = world->getAgent(type.toStdString(), agentName.toStdString());
            vector<string> attributes = agent->getAttributeList();
            for(int k=0; k < attributes.size(); ++k){
                QString attribName = QString::fromStdString(attributes[k]);
                QStandardItem *attribItem = new QStandardItem(attribName);

                QString value = getAgentValue(agent, attribName.toStdString());
                QStandardItem *valueItem = new QStandardItem(value);
                valueItem->setEditable(false);
                attribItem->setEditable(false);
                QList<QStandardItem*> l;
                l.append(attribItem);
                l.append(valueItem);
                child->appendRow(l);

            }
            item->appendRow(child);
        }
        agentModel->setItem(i,0, item);
    }

    agentModel->setHorizontalHeaderItem( 0, new QStandardItem( "Agents" ) );
    agentModel->setHorizontalHeaderItem( 1, new QStandardItem( "Value" ) );
    QObject::connect(agentModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(agentItemChange(QStandardItem*)));
    ui->agentView->setModel(agentModel);
    ui->agentView->setColumnWidth(0, 200);
    QItemSelectionModel* selection = ui->agentView->selectionModel();
    QObject::connect(selection, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(selectedAgentItem(const QItemSelection&, const QItemSelection&)));
}



