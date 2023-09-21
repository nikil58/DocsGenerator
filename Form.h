#pragma once

#include <QVBoxLayout>
#include <QTabWidget>
#include <QLineEdit>
#include <QWebView>
#include <QTextEdit>
#include <QApplication>
#include "PreviewWorker.h"

class Form : public QWidget {
Q_OBJECT
private:
    /// QFont used for define font-size of anything in form
    QFont font_size_;

    /// Layout for wrap main preview_widget_
    QVBoxLayout* form_layout_;

    /// Tab menu for form
    QTabWidget* tabs_;
    QLineEdit* title_field_;
    QTextEdit* inputs_field_;
    QTextEdit* const_field_;
    QTextEdit* algorithm_field_;
    QTextEdit* output_field_;
    QLineEdit* link_field_;

    QWebView* preview_widget_{};

    QTextEdit* last_selected_field_{};

    bool eventFilter(QObject* obj, QEvent* e);
    void DrawMainForm();

    const QString formulas_buttons_[13] = {"Формула", "Нижний", "Верхний", "Корень", "Дробь", "∆", "Сумма", "Произведение", "Интеграл", "X", "Pi", "<=", "Бесконечность"};
public:
    explicit Form();

private slots:
    void OperationClick();
    void CopyButtonClicked();
    void Rerender(QString text);
};