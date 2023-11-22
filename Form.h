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

    /// Layout for wrap all form
    QHBoxLayout* form_layout_;

    /// Layout for Copy and Clear formulas_buttons_
    QHBoxLayout* buttons_layout_{};

    /// Tab menu for form
    QTabWidget* tabs_{};

    /**
     * Below are fields that used to fill the html doc
     * <br>
     * START
     */

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
    // END

    /// Horizontal line that split title input field from rest part of the form: formulas formulas_buttons_ and tabs
    QFrame* title_splitter_{};

    /// Grid layout that have 5 formulas_buttons_ in a row
    QGridLayout* formulas_buttons_layout_{};

    /// Left-side widget that imitate Chromium browser and displays HTML
    QWebEngineView* preview_widget_{};

    /// Pointer to last selected field. It useful when we clicked on the formulas formulas_buttons_ and insert text to the last input
    QTextEdit* last_selected_field_{};

    /// Custom class that generate html text from form's inputs fields in the separate thread
    PreviewWorker* worker_{};

    /// For WebEngine we use text that stores in the file: it has a lot css and spans. But we want copy less part, so here it is
    QString text_in_preview_{};

    /**
     * @brief Handler for saving the last_selected_field pointer
     * @param obj event sender
     * @param e event
     */
    bool eventFilter(QObject* obj, QEvent* e) override;

    /**
     * @brief Method that draw everything except tabs
     */
    void DrawMainForm();

    /**
     * @brief Method that draw module tab
     * @return Widget that will be insert to the TabWidget
     */
    QWidget* DrawFirstTab();

    /**
     * @brief Method that draw modele tab
     * @return Widget that will be insert to the TabWidget
     */
    QWidget* DrawSecondTab();

    void AddSymbolToField(const QString& symbol, const QString& shift_count);

    /// List of all formulas formulas_buttons_
    const QVector<QVector<QString>> formulas_buttons_ = {{"Формула", "\\(\\)",   "2"}, {"sub", "_{}", "1"}, {"sup", "^{}", "1"},
                                                         {"root",    "\\sqrt{}", "1"}, {"frac", "\\frac{}{}", "3"}, {"Delta", "\\Delta", "0"},
                                                         {"sum",     "\\sum{}",  "1"}, {"prod", "\\prod{}", "1"}, {"int", "\\int", "0"}, {"vec", "\\times", "0"},
                                                         {"pi",      "\\pi",     "0"}, {"alpha", "\\alpha", "0"}, {"beta", "\\beta", "0"}, {"phi", "\\varphi", "0"},
                                                         {"theta",   "\\theta",  "0"}, {"leq", "\\leq", "0"}, {"inf", "\\infty", "0"}, {"Фотография", "", "0"}};
public:
    explicit Form();
    ~Form() override;
signals:
    /// Signal that emits when Clear formulas_buttons_ is clicked for worker
    void ClearCache();

private slots:
    /// Slot that invoke on every formulas button click
    void OperationClick();
    /// Slot that invokes on every Clear button click
    void ClearButtonClicked();
    /// Slot that invokes on every Copy button click
    void CopyButtonClicked();
    /// Slot that invokes  on every signals "RenderIsReady" from Worker
    void Rerender(QString text);
    /// Slot that invokes on every switch tab action
    void SwitchTab(int index);
    /// Slot that invokes when link is clicked in the preview widget
    void ClickOnLink(const QUrl &);
    /// Slot that invokes every time when File->Import is clicked
    void ImportFile(bool);
    /// Slot that invokes every time when File->Export is clicked
    void ExportFile(bool);
};