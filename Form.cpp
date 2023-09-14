#include "Form.h"

#include <QLabel>
#include <QPushButton>
#include <QWebFrame>
#include <QThread>
#include <QEvent>
#include <QClipboard>

#include <QSplitter>

Form::Form() {
    this->setWindowFlags(Qt::Window/* | Qt::WindowStaysOnTopHint*/);
    this->setWindowTitle("Генератор документации");
    this->setMinimumSize(1280, 720);

    font_size_.setPixelSize(20);
    form_layout_ = new QVBoxLayout(this);
    DrawMainForm();

    this->show();
}

void Form::DrawMainForm() {
    title_field_ = new QLineEdit();
    title_field_->setMaxLength(20);
    title_field_->setFont(font_size_);

    QLabel* label = new QLabel("Название");
    label->setFont(font_size_);

    auto under_line = new QFrame();
    under_line->setFrameShape(QFrame::HLine);
    under_line->setFrameShadow(QFrame::Sunken);

    QHBoxLayout* buttons_layout = new QHBoxLayout();
    for (int i = 0; i < 7; i++) {
        QPushButton* btn = new QPushButton("Low");
        btn->setFont(font_size_);
        buttons_layout->addWidget(btn);
        connect(btn, SIGNAL(clicked()), this, SLOT(OperationClick()));
    }

    QLabel* input_label = new QLabel("Входы");
    input_label->setFont(font_size_);
    inputs_field_ = new QTextEdit();
    inputs_field_->setFont(font_size_);
    input_label->setBuddy(inputs_field_);

    QLabel* const_label = new QLabel("Константы");
    const_label->setFont(font_size_);
    const_field_ = new QTextEdit();
    const_field_->setFont(font_size_);
    const_label->setBuddy(const_field_);

    QLabel* algorithm_label = new QLabel("Алгоритм");
    algorithm_label->setFont(font_size_);
    algorithm_field_ = new QTextEdit();
    algorithm_field_->setFont(font_size_);
    algorithm_label->setBuddy(algorithm_field_);

    QLabel* output_label = new QLabel("Выход");
    output_label->setFont(font_size_);
    output_field_ = new QTextEdit();
    output_field_->setFont(font_size_);
    output_label->setBuddy(output_field_);

    QLabel* link_label = new QLabel("Ссылка на пример");
    link_label->setFont(font_size_);
    link_field_ = new QLineEdit();
    link_field_->setFont(font_size_);
    link_label->setBuddy(link_field_);

    QString test = "<!DOCTYPE html>\n"
                   "<html><head></head><body><div><div><font size=\\\"6\\\"><b>A.8.1 Имитация шумов дальнего судоходства</b></font></div><div><font size=\\\"5\\\"><br></font></div><div><font size=\\\"5\\\"><br></font></div><div><font size=\\\"5\\\"><b>Входы</b>:</font></div><div><font size=\\\"5\\\"><br></font></div><div><ul><li><font size=\\\"5\\\"><i>f</i>&nbsp;- массив частот, кГц,&nbsp;</font><font size=\\\"5\\\"><math><mrow><mi><br></mi><mi>f</mi></mrow></math>.</font></li><li><font size=\\\"5\\\"><i>ft</i>&nbsp;- частота, соответствующая максимуму&nbsp;</font><font size=\\\"5\\\"><math><msub><mi>P</mi><mrow><mi mathvariant=\\\"normal\\\">т</mi></mrow></msub></math>, кГц, <math><mrow><msub><mi>f</mi><mrow><mi>т</mi></mrow></msub></mrow></math>.</font></li><li><font size=\\\"5\\\"><i>pt0</i>&nbsp;- значение этого максимума, определяемое глубиной места и плотностью судоходства, Па,&nbsp;</font><font size=\\\"5\\\"><math><msub><mi>P</mi><mrow><mi mathvariant=\\\"normal\\\">т</mi><mn>0</mn></mrow></msub></math>.</font></li></ul></div><div><font size=\\\"5\\\"><br></font></div></div><div><font size=\\\"5\\\"><br></font></div><div><font size=\\\"5\\\"><b>Выходы</b>:</font></div><div><font size=\\\"5\\\"><br></font></div><div><ul><li><font size=\\\"5\\\"><i>p0</i>&nbsp;- спектральный уровень шумов судоходства, Па,<math><mrow><msub><mi>P</mi><mrow><mn>0</mn><mi mathvariant=\\\"normal\\\">с</mi><mi mathvariant=\\\"normal\\\">у</mi><mi mathvariant=\\\"normal\\\">д</mi></mrow></msub><mrow><mo stretchy=\\\"false\\\">(</mo><mi>f</mi><mo stretchy=\\\"false\\\">)</mo></mrow></mrow></math></font></li></ul></div><div><span style=\\\"font-size: x-large;\\\"><br></span></div><br><br><font size=\\\"5\\\">Спектральный уровень шумов судохоства рассчитывается с помощью формулы:<br><br><math><mrow><mrow><msub><mi>P</mi><mrow><mn>0</mn><mi mathvariant=\\\"normal\\\">с</mi><mi mathvariant=\\\"normal\\\">у</mi><mi mathvariant=\\\"normal\\\">д</mi></mrow></msub><mrow><mo stretchy=\\\"false\\\">(</mo><mi>f</mi><mo stretchy=\\\"false\\\">)</mo></mrow></mrow><mo>=</mo><msqrt><mfrac><mrow><mn>2</mn><mo>×</mo><mmultiscripts><mi>P</mi><mrow><mi mathvariant=\\\"normal\\\">т</mi><mn>0</mn></mrow><none></none><none></none><mn>2</mn></mmultiscripts><mo>×</mo><mmultiscripts><mi>f</mi><mi mathvariant=\\\"normal\\\">т</mi><none></none><none></none><mn>3</mn></mmultiscripts><mo>×</mo><msup><mi>f</mi><mn>2</mn></msup></mrow><mrow><msup><mi>f</mi><mn>5</mn></msup><mo>+</mo><mmultiscripts><mi>f</mi><mi mathvariant=\\\"normal\\\">т</mi><none></none><none></none><mn>5</mn></mmultiscripts></mrow></mfrac></msqrt></mrow></math></font></body></html>";

    preview_widget_ = new QWebView();
    preview_widget_->setMinimumWidth(300);
    preview_widget_->setHtml(test);
    preview_widget_->setDisabled(true);

    QPushButton* copy_button = new QPushButton("Копировать");
    copy_button->setFont(font_size_);
    connect(copy_button, SIGNAL(clicked()), this, SLOT(CopyButtonClicked()));

    QVBoxLayout* preview_button_layout = new QVBoxLayout();
    preview_button_layout->addWidget(copy_button);
    preview_button_layout->addWidget(preview_widget_);
    QWidget* right_container = new QWidget();
    right_container->setLayout(preview_button_layout);

    QVBoxLayout* left_side_menu = new QVBoxLayout();
    left_side_menu->addWidget(label);
    left_side_menu->addWidget(title_field_);
    left_side_menu->addWidget(under_line);
    left_side_menu->addLayout(buttons_layout);
    left_side_menu->addWidget(input_label);
    left_side_menu->addWidget(inputs_field_);
    left_side_menu->addWidget(const_label);
    left_side_menu->addWidget(const_field_);
    left_side_menu->addWidget(algorithm_label);
    left_side_menu->addWidget(algorithm_field_);
    left_side_menu->addWidget(output_label);
    left_side_menu->addWidget(output_field_);
    left_side_menu->addWidget(link_label);
    left_side_menu->addWidget(link_field_);

    QSplitter* splitter = new QSplitter();
    splitter->setChildrenCollapsible(false);
    splitter->setOrientation(Qt::Horizontal);

    QWidget* left_side_container = new QWidget();
    left_side_container->setLayout(left_side_menu);
    splitter->addWidget(left_side_container);
    splitter->addWidget(right_container);
    splitter->setHandleWidth(2);
    splitter->setFrameShadow(QFrame::Sunken);

    connect(title_field_, SIGNAL(textChanged(const QString &)), SLOT(UpdatePreview()));
    connect(inputs_field_, SIGNAL(textChanged()), SLOT(UpdatePreview()));
    connect(const_field_, SIGNAL(textChanged()), SLOT(UpdatePreview()));
    connect(algorithm_field_, SIGNAL(textChanged()), SLOT(UpdatePreview()));
    connect(output_field_, SIGNAL(textChanged()), SLOT(UpdatePreview()));
    connect(link_field_, SIGNAL(textChanged(const QString &)), SLOT(UpdatePreview()));

    inputs_field_->installEventFilter(this);
    const_field_->installEventFilter(this);
    algorithm_field_->installEventFilter(this);
    output_field_->installEventFilter(this);



    tabs_ = new QTabWidget();
    tabs_->setFont(font_size_);
    tabs_->addTab(splitter, "Модуль");
    form_layout_->addWidget(tabs_);
}

void Form::UpdatePreview() {
    QString title_start = "<html><head></head><body><h1><font face=\\\"Times New Roman, serif\\\"><span style=\\\"font-size: 16px;\\\">";
    QString title = title_field_->text().replace("\n", "<br>");
    QString title_end = "</span></font></h1>";
    QString inputs_start = "<p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                           "        <u style=\\\"font-family: 'Times New Roman', serif; font-size: 16px;\\\">Вход:</u>\n"
                           "    </p>"
                           "<p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                           "    <font face=\\\"Liberation Serif, serif\\\">\n"
                           "        <font style=\\\"font-size: 12pt\\\"><font color=\\\"#000000\\\"><font face=\\\"Times New Roman, serif\\\">\n"
                           "        <i>";
    QString inputs = inputs_field_->toPlainText().replace("\n", "<br>");
    QString inputs_end = "</i>\n"
                         "        </font>\n"
                         "    </font></font></font></p>";
    QString const_start = "<p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                          "        <u style=\\\"font-family: 'Times New Roman', serif; font-size: 16px;\\\">Константы:</u>\n"
                          "    </p>\n"
                          "    <p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                          "    <font face=\\\"Liberation Serif, serif\\\">\n"
                          "    <font style=\\\"font-size: 12pt\\\"><font color=\\\"#000000\\\"><font face=\\\"Times New Roman, serif\\\">\n"
                          "        <i>";
    QString const_field = const_field_->toPlainText().replace("\n", "<br>");
    QString const_end = "</sub></i>\n"
                        "    </font>\n"
                        "    </font></font></font></p>";
    QString algorithm_start = "<p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                              "        <u style=\\\"font-family: 'Times New Roman', serif; font-size: 16px;\\\">Алгоритм:</u>\n"
                              "    </p>\n"
                              "    <p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                              "    <font face=\\\"Liberation Serif, serif\\\">\n"
                              "    <p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"text-align: center; line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm;\\\"><font face=\\\"Liberation Serif, serif\\\"><font style=\\\"font-size: 12pt\\\"><font color=\\\"#000000\\\"><font face=\\\"Times New Roman, serif\\\">";
    QString algorithm = algorithm_field_->toPlainText().replace("\n", "<br>");
    QString algorithm_end = "</font></font></font></font></p>";
    QString output_start = "<p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                           "        <u style=\\\"font-family: 'Times New Roman', serif; font-size: 16px;\\\">Выход:</u>\n"
                           "    </p>\n"
                           "    <p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                           "    <font face=\\\"Liberation Serif, serif\\\">\n"
                           "    <font style=\\\"font-size: 12pt\\\"><font color=\\\"#000000\\\"><font face=\\\"Times New Roman, serif\\\">\n"
                           "        <i>";
    QString output = output_field_->toPlainText().replace("\n", "<br>");
    QString output_end = "</i>\n"
                         "    </font>\n"
                         "    </font></font></font></p>";
    QString link_start = "<p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                         "        <u style=\\\"font-family: 'Times New Roman', serif; font-size: 16px;\\\"><a href=\\";
    QString link = link_field_->text().replace("\n", "<br>");
    QString link_end = "\\\">Пример использования</a></u>\n"
                       "    </p>";
    preview_widget_->setHtml(title_start + title + title_end + inputs_start + inputs.replace(".*\\n.*","<br>") + inputs_end + const_start + const_field + const_end + algorithm_start + algorithm + algorithm_end + output_start +output + output_end + link_start + link + link_end);
}

void Form::OperationClick() {
    last_selected_field_->insertPlainText("#low()$");
    last_selected_field_->moveCursor(QTextCursor::Left);
    last_selected_field_->moveCursor(QTextCursor::Left);
    last_selected_field_->setFocus();
}

bool Form::eventFilter(QObject* obj, QEvent* e) {
    if (e->type() == QEvent::FocusOut) {
        QTextEdit* text_edit = qobject_cast<QTextEdit*>(obj);
        if (text_edit)
            last_selected_field_ = text_edit;
    }
    return false;
}

void Form::CopyButtonClicked(){
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(preview_widget_->page()->mainFrame()->toHtml(),QClipboard::Clipboard);

    if (clipboard->supportsSelection()) {
        clipboard->setText(preview_widget_->page()->mainFrame()->toHtml(), QClipboard::Selection);
    }

#if defined (Q_OS_LINUX)
    QThread::msleep(1);
#endif

}