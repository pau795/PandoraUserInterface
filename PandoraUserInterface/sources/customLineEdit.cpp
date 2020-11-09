#include "customLineEdit.h"


CustomLineEdit::CustomLineEdit()
{

}

double CustomLineEdit::getMinValue(){
    return minValue;
}
double CustomLineEdit::getMaxValue(){
    return maxValue;
}

void CustomLineEdit::setMinValue(double value){
    this->minValue = value;
}

void CustomLineEdit::setMaxValue(double value){
    this->maxValue = value;
}

void CustomLineEdit::setCustomValue(int value){
    double d = minValue + (((maxValue-minValue)/99) * value);
    QString currentText = this->text();
    bool isInt = false;
    currentText.toInt(&isInt);
    QString s;
    if (isInt) s =QString::number(d,'f',0);
    else s = QString::number(d, 'f', 2);
    this->setText(s);
}

void CustomLineEdit::checkValues(){
    double value = this->text().toDouble();
    if (value <= minValue or value >= maxValue) { // outside range -> set new proper range
        minValue = powf(10.0f, floorf(log10f(value)));
        maxValue = powf(10.0f, floorf(log10f(value)+1));
        if (value == 0){
            minValue = 0;
            maxValue = 10;
        }
        emit minValueChanged(minValue);
        emit maxValueChanged(maxValue);
    }
    double chunk = (maxValue-minValue)/99;  // get the proper slider position with the new value
    double d = (value - minValue)/chunk;
    emit customValueChanged(int(d));
}
