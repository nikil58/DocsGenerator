#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QFrame>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include "ToggleButtonR.h"

class Spoiler : public QWidget {
    Q_OBJECT
private:
    QGridLayout main_layout_;
    ToggleButtonR toggle_button_;
    QFrame header_line_;
    QParallelAnimationGroup toggle_animation_;
    QScrollArea content_area_;
    int animation_duration_{300};
    QWidget* parent_;
    void OffButton();
public:
    explicit Spoiler(const QString& title = "", QWidget* parent = nullptr, const int animation_duration = 300);
    void SetContentLayout(QLayout& content_layout);
    void ToggleButton();
};
