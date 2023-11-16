#pragma once

#include <QVBoxLayout>
#include <QTabWidget>
#include <QLineEdit>
#include <QWebEngineView>
#include <QTextEdit>
#include <QApplication>
#include <QSplitter>
#include <QLabel>
#include <QMainWindow>
#include "PreviewWorker.h"

class Form : public QMainWindow {
Q_OBJECT
private:
    /// QFont used for define font-size of anything in form
    QFont font_size_;

    /// Layout for wrap main preview_widget_
    QHBoxLayout* form_layout_;

    QHBoxLayout* buttons_layout_{};

    /// Tab menu for form
    QTabWidget* tabs_{};

    QLineEdit* title_field_{};
    QLabel* title_label_{};
    QTextEdit* inputs_field_{};
    QTextEdit* const_field_{};
    QTextEdit* algorithm_field_{};
    QTextEdit* output_field_{};
    QLineEdit* link_field_1_{};

    QTextEdit* input_description_field_{};
    QTextEdit* input_list_field_{};
    QTextEdit* output_description_field_{};
    QTextEdit* output_list_field_{};
    QLineEdit* link_field_2_{};
    QLineEdit* section_name_{};
    QTextEdit* section_field_{};

    QFrame* title_splitter_{};

    QGridLayout* formulas_buttons_layout_{};

    QWebEngineView* preview_widget_{};

    QTextEdit* last_selected_field_{};

    PreviewWorker* worker_{};

    QString text_in_preview_{};

    bool eventFilter(QObject* obj, QEvent* e) override;
    void DrawMainForm();
    QWidget* DrawFirstTab();
    QWidget* DrawSecondTab();

    const QString formulas_buttons_[18] = {"Формула", "sub", "sup", "root", "frac", "Delta", "sum", "prod", "int", "vec", "pi", "alpha", "beta", "phi", "theta", "leq", "inf", "Фотография"};
public:
    explicit Form();
    ~Form() override;
signals:
    void ClearCache();

private slots:
    void OperationClick();
    void ClearButtonClicked();
    void CopyButtonClicked();
    void Rerender(QString text);
    void SwitchTab(int index);
    void ClickOnLink(const QUrl &);
    void ImportFile(bool);
    void ExportFile(bool);
};