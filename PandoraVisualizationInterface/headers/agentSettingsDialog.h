#ifndef AGENTSETTINGSDIALOG_H
#define AGENTSETTINGSDIALOG_H

#include <QDialog>
#include "shape.h"
#include <iostream>

namespace Ui {
class AgentSettingsDialog;
}

class AgentSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AgentSettingsDialog(Shape::ShapeSettings &settings, QWidget *parent = nullptr);
    ~AgentSettingsDialog();

signals:
    void settingsChanged(Shape::ShapeSettings &st);

public slots:

    void shapeSelectionChanged(const QModelIndex &i);
    void colorSelectionChanged(const QModelIndex &i);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AgentSettingsDialog *ui;

    const static int numColors = 8;
    const static QString colorNames[numColors];
    Shape::ShapeSettings settings;

};

#endif // AGENTSETTINGSDIALOG_H
