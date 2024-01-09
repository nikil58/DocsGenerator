#include <QPropertyAnimation>
#include <QDebug>
#include "Spoiler.h"
Spoiler::Spoiler(const QString& title, const int animation_duration, QWidget *parent) : QWidget(parent), animation_duration_(animation_duration) {
    QFont font;
    font.setPixelSize(20);

    toggle_button_.setStyleSheet("QToolButton { border: none; }");
    toggle_button_.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggle_button_.setArrowType(Qt::ArrowType::RightArrow);
    toggle_button_.setText(title);
    toggle_button_.setFont(font);
    toggle_button_.setCheckable(true);
    toggle_button_.setChecked(false);

    header_line_.setFrameShape(QFrame::HLine);
    header_line_.setFrameShadow(QFrame::Sunken);
    header_line_.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    content_area_.setStyleSheet("QScrollArea { background-color: transparent; border: none; }");
    content_area_.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // start out collapsed
    content_area_.setMaximumHeight(0);
    content_area_.setMinimumHeight(0);
    // let the entire widget grow and shrink with its content
    toggle_animation_.addAnimation(new QPropertyAnimation(this, "minimumHeight"));
    toggle_animation_.addAnimation(new QPropertyAnimation(this, "maximumHeight"));
    toggle_animation_.addAnimation(new QPropertyAnimation(&content_area_, "maximumHeight"));
    // don't waste space
    main_layout_.setVerticalSpacing(0);
    main_layout_.setContentsMargins(0, 0, 0, 0);
    int row = 0;
    main_layout_.addWidget(&toggle_button_, row, 0, 1, 1, Qt::AlignLeft);
    main_layout_.addWidget(&header_line_, row++, 2, 1, 1);
    main_layout_.addWidget(&content_area_, row, 0, 1, 3);
    setLayout(&main_layout_);
    QObject::connect(&toggle_button_, &QToolButton::clicked, [this](const bool checked) {
        toggle_button_.setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
        toggle_animation_.setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        toggle_animation_.start();
    });
}

void Spoiler::SetContentLayout(QLayout & content_layout) {
    delete content_area_.layout();
    content_area_.setLayout(&content_layout);
    const auto collapsedHeight = sizeHint().height() - content_area_.maximumHeight();
    auto contentHeight = content_layout.sizeHint().height();
    for (int i = 0; i < toggle_animation_.animationCount() - 1; ++i) {
        QPropertyAnimation * spoilerAnimation = static_cast<QPropertyAnimation *>(toggle_animation_.animationAt(i));
        spoilerAnimation->setDuration(animation_duration_);
        spoilerAnimation->setStartValue(collapsedHeight);
        spoilerAnimation->setEndValue(collapsedHeight + contentHeight);
    }
    QPropertyAnimation * contentAnimation = static_cast<QPropertyAnimation *>(toggle_animation_.animationAt(toggle_animation_.animationCount() - 1));
    contentAnimation->setDuration(animation_duration_);
    contentAnimation->setStartValue(0);
    contentAnimation->setEndValue(contentHeight);
}

void Spoiler::ToggleButton() {
    toggle_button_.click();
}