#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <math.h>
using namespace std;

class CustomLineEdit : public QLineEdit
{
        Q_OBJECT
    public:
        CustomLineEdit();

        double getMinValue();
        double getMaxValue();

        void setMinValue(double value);
        void setMaxValue(double value);


    signals:
        void customValueChanged(int spinnerValue);
        void minValueChanged(double minValue);
        void maxValueChanged(double maxValue);

    public slots:
        void setCustomValue(int value);
        void checkValues();

    private:
        double minValue, maxValue;
};

#endif // CUSTOMLINEEDIT_H
