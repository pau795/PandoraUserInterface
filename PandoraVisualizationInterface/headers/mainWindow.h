#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customGLWidget.h"
#include "simulationWorld.h"
#include "agentSettingsDialog.h"
#include <QTimer>
#include <QFileDialog>
#include <QStandardItemModel>
#include <iostream>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadSimulation();


private slots:
    void on_actionTerrain_Elevation_triggered();

    void on_actionReset_View_triggered();

    void on_actionPlay_triggered();

    void on_actionOpen_Simulation_triggered();

    void agentItemChange(QStandardItem* item);

    void rasterItemChange(QStandardItem* item);

    void selectedAgentItem(const QItemSelection& selected, const QItemSelection& deselected);

    void setCurrentStep(int step);

    void on_actionReset_Simulation_triggered();

    void on_agentView_expanded(const QModelIndex &index);

    void timerTick();

    void on_actionForward_triggered();

    void on_actionRewind_triggered();

    void on_actionSet_timer_triggered();

    void onCustomContextMenu(const QPoint &point);

    void on_actionSet_agent_type_figure_triggered();

    void agentSettingsChanged(Shape::ShapeSettings &settings);

private:
    Ui::MainWindow *ui;
    CustomGLWidget *gl;
    QStandardItemModel *rasterModel, *agentModel;
    QString modelPath;
    SimulationWorld* world;
    int numSteps, cStep;
    int rows, cols;
    int numAgents;
    map<string, Shape::ShapeSettings> agentSettings;
    map<string, bool> rasterSettings;

    QModelIndex agentTypeIndex;
    QMenu *agentTypeMenu;
    QTimer *timer;
    int stepsPerSecond;
    int stepInc;
    bool play;
    void setRasterView();
    void setAgentView();
    void updateAgentsValue();
    void setupSimulationUI();
    void deleteRasterModel();
    void deleteAgentModel();
    QString getAgentValue(SimulationAgent* agent, string attribute);
    string getRasterToDraw();

};
#endif // MAINWINDOW_H
