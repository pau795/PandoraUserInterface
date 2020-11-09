#ifndef EXECUTABLERUNNER_H
#define EXECUTABLERUNNER_H

#include <QObject>
#include <QProcess>
#include <QTextEdit>
#include <iostream>

using namespace std;

class ExecutableRunner: public QObject
{
    Q_OBJECT

public:

    ExecutableRunner(QString dir, QString executable);
    void runProcess();
    QString getCurrentLog();
    void killProcess();


public slots:
    void processMessage();
    void processResult(int exitCode, QProcess::ExitStatus exitStatus);

signals:
    void newMessage(QString message);
    void processFinished(int exitCode);


private:
    QProcess* process;
    QString log;
    QString workingDir;
    QString executable;
    bool finished;


};

#endif // EXECUTABLERUNNER_H
