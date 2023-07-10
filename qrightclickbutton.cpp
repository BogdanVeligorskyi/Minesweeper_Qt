#include "qrightclickbutton.h"

QRightClickButton::QRightClickButton(QWidget *parent) :
    QPushButton(parent)
{
}

void QRightClickButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton)
        emit rightClicked();
    QPushButton::mousePressEvent(e);
}
