#include "qslidercustom.h"

QSliderCustom::QSliderCustom(QSlider *parent) : QSlider(parent)
{

}

QSliderCustom::QSliderCustom(Qt::Orientation orientation, QWidget *parent)
{
    setOrientation(orientation);
}

void QSliderCustom::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    double k = ((double)event->x())/((double)width());
    int a = k*(maximum());
    setValue(a);
    emit pressedEv(a);
}


