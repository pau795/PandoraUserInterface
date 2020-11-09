#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <QWidget>
#include <QString>
#include <QGroupBox>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
#include <QTextStream>
#include <QDir>
#include "wheelEventFilter.h"
#include "tinyxml.h"
#include "customLineEdit.h"
#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include "utils.h"
#include <math.h>


using namespace std;

TiXmlDocument loadConfig(string path);
QGroupBox* treeRecursion(TiXmlElement* root);
QGroupBox* attributeGroupBox(TiXmlAttribute* attrib);

void loadParameters(QWidget* parameterContainer, TiXmlElement* root);
void setToZero(TiXmlElement* root);

void writeChanges(TiXmlDocument* doc, QWidget* container);
void writeRecursion(TiXmlElement* node, QObject* box);

void writeIncrement(TiXmlDocument* doc, QWidget* container);
void writeIncrementRecursion(TiXmlElement* node, QObject* box);

void showIncrementWidget(QWidget* container, bool show);
void showIncrementRecursion(QObject* box, bool show);

void increaseConfigFile(TiXmlElement* configNode, TiXmlElement* incrementNode);

int getNumSteps(TiXmlElement* root);
QString getDirPath(QString filePath);
QString findExecutable(QString dir);
bool fileExists(QString filePath);


#endif // CONFIGLOADER_H
