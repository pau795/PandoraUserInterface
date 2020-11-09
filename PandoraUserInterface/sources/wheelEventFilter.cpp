#include "wheelEventFilter.h"

wheelEventFilter::wheelEventFilter(QObject *parent) : QObject(parent)
{

}

bool wheelEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Wheel) {
        //ignore wheel event
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
