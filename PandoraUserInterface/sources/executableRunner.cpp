#include "executableRunner.h"

ExecutableRunner::ExecutableRunner(QString workingDirectory, QString executable){
    workingDir = workingDirectory;
    this->executable = executable;
    log = "";
    process = new QProcess();
    QObject::connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(processMessage()));
    connect(process , SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processResult(int, QProcess::ExitStatus)));
    connect(process , SIGNAL(finished(int,QProcess::ExitStatus)), this, SIGNAL(processFinished(int)));
}

void ExecutableRunner::runProcess(){
    process->setWorkingDirectory(workingDir);
    process->start(executable);
}

QString ExecutableRunner::getCurrentLog(){
    return log;
}

void  ExecutableRunner::processMessage(){
    QString output = process->readAll();
    emit newMessage(output);
    log.append(output);
}

void ExecutableRunner::killProcess(){
    process->kill();
}

void ExecutableRunner::processResult(int exitCode, QProcess::ExitStatus exitStatus){
    log.append("Exit Code: " +  QString::number(exitCode)+"\n");
    emit newMessage("Exit Code: " +  QString::number(exitCode)+"\n");
    if (exitStatus==QProcess::NormalExit){
        log.append(executable + " finished correctly.");
        emit newMessage(executable + " finished correctly.");
    }
    else {
        log.append("An error ocurred during " + executable + " execution.");
        emit newMessage("An error ocurred during " + executable + " execution.");
    }
}
