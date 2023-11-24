#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QFrame>
#include <QParallelAnimationGroup>
#include <QScrollArea>

class Spoiler : public QWidget {
    Q_OBJECT
private:
    QGridLayout main_layout_;
    QToolButton toggle_button_;
    QFrame header_line_;
    QParallelAnimationGroup toggle_animation_;
    QScrollArea content_area_;
    int animation_duration_{300};
public:
    explicit Spoiler(const QString& title = "", const int animation_duration = 300, QWidget* parent = 0);
    void SetContentLayout(QLayout& content_layout);
    void ToggleButton();
};
