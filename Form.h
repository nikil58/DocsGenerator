#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QTabWidget>

class Form : public QWidget {
private:
    /// QFont used for define font-size of anything in form
    QFont font_size_;

    /// Layout for wrap main widget
    QVBoxLayout* form_layout_;

    /// Tab menu for form
    QTabWidget* tabs_{};

    void DrawMainForm();
public:
    explicit Form();

};