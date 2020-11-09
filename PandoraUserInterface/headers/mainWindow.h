#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QObject>
#include <QScrollArea>
#include "tinyxml.h"
#include "configLoader.h"
#include "executableRunner.h"
#include "runningSimulationWindow.h"
#include <QFileDialog>
#include <QIntValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void readyToRunModel(QString dir, QString executable);

private slots:

    void on_BrowserButton_clicked();

    void on_saveButton_clicked();

    void on_runButton_clicked();

    void on_consoleCheckBox_clicked(bool checked);

    void on_actionSave_Model_triggered();

    void on_actionCompile_Model_triggered();

    void on_actionRun_Model_triggered();

    void on_actionClear_Console_triggered();

    void on_actionOpen_Model_triggered();

    void on_compileCheckBox_stateChanged(int arg1);

    void on_actionCompile_Model_before_running_triggered(bool checked);

    void on_actionShow_Console_triggered(bool checked);

    void compilationFinished(int exitCode);

    void runModel(QString dir, QString executable);

    void executableMessage(QString message);

    void activateWindow();

    void displayIncrement(bool show);

    void on_singleRunRButton_clicked();

    void on_multipleRunsRButton_clicked();

private:
    Ui::MainWindow *ui;

    QWidget* parameterContainer;
    QString modelPath;
    QString modelExecutable;
    QString modelDir;
    ExecutableRunner* compiler;

    TiXmlDocument *configFile;
    TiXmlDocument *incrementFile;

    int simulationSteps;
    int numSimulations;
    bool incrementVisible;
    bool runModelAfterCompile;

    void clearLayout(QLayout* layout);
    void compileModel(QString dir);


};
#endif // MAINWINDOW_H
