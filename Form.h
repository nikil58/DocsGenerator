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
#include <QPushButton>
#include "PreviewWorker.h"
#include "CharacterForm.h"

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

    /// Left-side widget that imitate Chromium browser and displays HTML
    QWebEngineView* preview_widget_{};

    /// Pointer to last selected field. It useful when we clicked on the formulas formulas_buttons_ and insert text to the last input
    QTextEdit* last_selected_field_{};

    /// Custom class that generate html text from form's inputs fields in the separate thread
    PreviewWorker* worker_{};

    /// For WebEngine we use text that stores in the file: it has a lot css and spans. But we want copy less part, so here it is
    QString text_in_preview_{};

    /// Current opened file name with .ini extension
    QString open_file_name_{};

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

    /**
     * @brief Method that update title of Main Window according on information that contains in open_file_name_
     */
    void UpdateTitle();

    /// List of all formulas formulas_buttons_
    const QVector<QVector<QString>> formulas_buttons_ = {{"Формула", "\\(\\)",   "2"}, {"sub", "_{}", "1"}, {"sup", "^{}", "1"},
                                                         {"root",    "\\sqrt{}", "1"}, {"frac", "\\frac{}{}", "3"}, {"Delta", "\\Delta", "0"},
                                                         {"sum",     "\\sum{}",  "1"}, {"prod", "\\prod{}", "1"}, {"int", "\\int", "0"}, {"times", "\\times", "0"},
                                                         {"pi",      "\\pi",     "0"}, {"alpha", "\\alpha", "0"}, {"beta", "\\beta", "0"}, {"phi", "\\varphi", "0"},
                                                         {"theta",   "\\theta",  "0"}, {"leq", "\\leq", "0"}, {"infty", "\\infty", "0"}, {"Фотография", "", "0"},
                                                         {"pm", "\\pm", "0"}, {"mp", "\\mp", "0"}, {"cdot", "\\cdot", "0"}, {"div", "\\div", "0"}, {"bigoplus", "\\bigoplus", "0"},
                                                         {"bigominus", "\\usepackage{mathabx}\\bigominus", "0"}, {"bigodot", "\\bigodot", "0"}, {"bigotimes", "\\bigotimes", "0"},
                                                         {"neg", "\\neg", "0"}, {"lor", "\\lor", "0"}, {"land", "\\land", "0"}, {"neq", "\\neq", "0"}, {"leq", "\\leq", "0"},
                                                         {"geq","\\geq", "0"}, {"ll", "\\ll", "0"}, {"gg", "\\gg", "0"}, {"approx", "\\approx", "0"}, {"sim", "\\sim", "0"},
                                                         {"simeq", "\\simeq", "0"}, {"equiv", "\\equiv", "0"}, {"perp", "\\perp", "0"}, {"parallel", "\\parallel", "0"},
                                                         {"mid", "\\mid", "0"}, {"in", "\\in", "0"}, {"notin", "\\notin", "0"}, {"ni", "\\ni", "0"}, {"cap", "\\cap", "0"},
                                                         {"cup", "\\cup", "0"}, {"subset", "\\subset", "0"}, {"supset", "\\supset", "0"}, {"subseteq", "\\subseteq", "0"},
                                                         {"supseteq", "\\supseteq", "0"}, {"notsubset", "\\not\\subset", "0"}, {"notsupset", "\\not\\supset", "0"},
                                                         {"mathbb", "\\usepackage{amsfonts}\\mathbb{}", "1"}, {"|x|", "\\left|\\right|", "7"}, {"cos(x)", "\\cos()", "1"},
                                                         {"sin(x)", "\\sin()", "1"}, {"tan(x)", "\\tan()", "1"}, {"arccos(x)", "\\arccos()", "1"}, {"arcsin(x)", "\\arcsin()", "1"},
                                                         {"arctan(x)", "\\arctan()", "1"}, {"lim", "\\lim{}", "1"}, {"dot", "\\dot{}", "1"}, {"overline", "\\overline{}", "1"},
                                                         {"overrightarrow", "\\overrightarrow{}", "1"}, {"widetilde", "\\widetilde{}", "1"}, {"gamma", "\\gamma", "0"},
                                                         {"delta", "\\delta", "0"}, {"epsilon", "\\epsilon", "0"}, {"varepsilon", "\\varepsilon", "0"}, {"zeta", "\\zeta", "0"},
                                                         {"eta", "\\eta", "0"}, {"vartheta", "\\vartheta", "0"}, {"Theta", "\\Theta", "0"}, {"iota", "\\iota", "0"},
                                                         {"lambda", "\\lambda", "0"}, {"Lambda", "\\Lambda", "0"}, {"mu", "\\mu", "0"}, {"limv2", "\\lim\\limits_{x \\to \\infty}{}", "1"},
                                                         {"intv2", "\\int\\limits_{a=0}^{a=b}{}", "1"}, {"sumv2", "\\sum\\limits_{a=0}^{a=b}{}","1"}, {"prodv2", "\\prod\\limits_{a=0}^{a=b}{}","1"}};

    CharacterForm* character_form_{};


    /**
     * @brief Method that finds path to Image folder in Etalon
     * @return {user's_path_to_etalon}/OM/Etalon/Resources/Images
     */
    static QString FindEtalonImagePath();

    /**
     * @brief Method that copies Image to the OM/Etalon/Resources/Images and show modal window with result status
     * @param path to Image which one should be copied
     */
    void CopyImageToEtalon(const QString& path);

    /**
     * @brief Method that checks what problem was invoked and inform user about it
     * @param source_path source path to image
     * @param destination_path path where the image should be copied
     * @param etalon_path path to the Images folder
     * @return String of final file destination
     */
    QString ProcessCopyFailure(const QString& source_path, QString destination_path, const QString& etalon_path);

    /**
     * @brief Method that finds file with cash path to the last used directory
     * @return file that contains path /home/{username}/.etalon/DocGeneratorPath.txt
     */
    [[nodiscard]] static QFile GetCachedFile() ;

    /**
     * @brief Method that gets path to the last used directory
     * @return string with path to the last used directory or path to DocsGenerator
     */
    static QString GetLastDirectoryPath();

    /**
     * @brief Method that sets path to the last used directory
     * @param path to the recently opened ini file
     */
    static void SetLastDirectoryPath(QString file_path);

    /**
     * @brief Method that write all data from QStringList with sep = ';' to QTextStream
     * @param list - QStringList, where gets data
     * @param stream stream, where write data
     *
     * @return true when write some data
     */
     static bool WriteCSV(const QStringList& list, QTextStream& stream);

    /**
     * @brief Method that connect all things up
     */
    void SetConnections();

    /**
     * @brief Method that disconnect SetChangedIndicator when ImportFile
     */
    void DisconnectIndicator();

    /**
     * @brief Method that connect SetChangedIndicator slot
     */
    void ConnectIndicator();

public:
    explicit Form();
    ~Form() override;

    static QString etalon_image_path_;

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
    /// Slot that invokes every time when File->Export ini is clicked
    void ExportIniFile(bool);
    /// Slot that invokes every time when File->Export csv is clicked
    void ExportCSVFile(bool);

    void OpenFormulas(bool);
    /// Slot that invokes when writing to fields for update the title
    void SetChangedIndicator();
};