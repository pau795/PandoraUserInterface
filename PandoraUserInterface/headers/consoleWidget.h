#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QTextEdit>
#include <QScrollBar>
#include "ansiescapecodehandler.h"

class ConsoleWidget : public QTextEdit
{
    Q_OBJECT

public:
    ConsoleWidget(QWidget *parent = nullptr);
    void addLine(QString s);


private:
    typedef QList<Utils::FormattedText> FormattedTextList;
};

#endif // CONSOLEWIDGET_H
