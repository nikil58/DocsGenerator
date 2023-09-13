#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLineEdit>
#include <QWebView>

class Form : public QWidget {
Q_OBJECT
private:
    /// QFont used for define font-size of anything in form
    QFont font_size_;

    /// Layout for wrap main widget
    QVBoxLayout* form_layout_;

    /// Tab menu for form
    QTabWidget* tabs_{};
    QLineEdit* line_edit{};
    QWebView* widget{};
    void DrawMainForm();
public:
    explicit Form();
private slots:
    void UpdatePreview();

};