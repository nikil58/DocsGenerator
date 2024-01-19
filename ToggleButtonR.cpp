#include "ToggleButtonR.h"

ToggleButtonR::ToggleButtonR(QWidget* parent) : QToolButton(parent){

}

void ToggleButtonR::mousePressEvent(QMouseEvent* e) {
    if (e->button()==Qt::RightButton) {
        emit rightClicked(isChecked());
    }
    QToolButton::mousePressEvent(e);
}