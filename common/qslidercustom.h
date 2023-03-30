#ifndef QSLIDERCUSTOM_H
#define QSLIDERCUSTOM_H

#include <QObject>
#include <QSlider>
#include <QMouseEvent>
#include <common/loghelper.h>

class QSliderCustom : public QSlider
{
    Q_OBJECT
public:
    explicit QSliderCustom(QSlider *parent = nullptr);
    explicit QSliderCustom(Qt::Orientation orientation, QWidget *parent = nullptr);

signals:
    void pressedEv(int pos);

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // QSLIDERCUSTOM_H
