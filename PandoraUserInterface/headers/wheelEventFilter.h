#ifndef WHEELEVENTFILTER_H
#define WHEELEVENTFILTER_H

#include <QObject>
#include <QEvent>

class wheelEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit wheelEventFilter(QObject *parent = nullptr);
    bool eventFilter(QObject *obj, QEvent *event);
signals:

};

#endif // WHEELEVENTFILTER_H
