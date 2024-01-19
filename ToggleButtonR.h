#pragma once

#include <QToolButton>
#include <QMouseEvent>
class ToggleButtonR : public QToolButton{
Q_OBJECT
public:
    explicit ToggleButtonR(QWidget* parent = nullptr);
private slots:
    void mousePressEvent(QMouseEvent* e);
signals:
    void rightClicked(bool checked = false);
};

