#include "Form.h"

#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

#include <QWebView>
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
    line_edit = new QLineEdit();
    line_edit->setMaxLength(20);
    line_edit->setFont(font_size_);
    line_edit->setText("test");

    QLabel* label = new QLabel("Test");
    label->setFont(font_size_);

    auto lineA = new QFrame();
    lineA->setFrameShape(QFrame::HLine);
    lineA->setFrameShadow(QFrame::Sunken);

    QHBoxLayout* buttons_layout = new QHBoxLayout();
    for (int i = 0; i < 7; i++) {
        QPushButton* btn = new QPushButton("Test");
        btn->setFont(font_size_);
        buttons_layout->addWidget(btn);
    }

    QLabel* input_label = new QLabel("Входы");
    input_label->setFont(font_size_);

    QTextEdit* input_text = new QTextEdit();
    input_text->setFont(font_size_);

    QLabel* const_label = new QLabel("Константы");
    const_label->setFont(font_size_);

    QTextEdit* const_text = new QTextEdit();
    const_text->setFont(font_size_);

    QLabel* algorithm_label = new QLabel("Алгоритм");
    algorithm_label->setFont(font_size_);

    QTextEdit* algorithm_text = new QTextEdit();
    algorithm_text->setFont(font_size_);

    QLabel* output_label = new QLabel("Выход");
    output_label->setFont(font_size_);

    QTextEdit* output_text = new QTextEdit();
    output_text->setFont(font_size_);

    QLabel* link_label = new QLabel("Ссылка на пример");
    output_label->setFont(font_size_);

    QLineEdit* link_text = new QLineEdit();
    link_text->setFont(font_size_);
    QString test = "<!DOCTYPE html>\n"
                   "<html><head></head><body><div><div><font size=\\\"6\\\"><b>A.8.1 Имитация шумов дальнего судоходства</b></font></div><div><font size=\\\"5\\\"><br></font></div><div><font size=\\\"5\\\"><br></font></div><div><font size=\\\"5\\\"><b>Входы</b>:</font></div><div><font size=\\\"5\\\"><br></font></div><div><ul><li><font size=\\\"5\\\"><i>f</i>&nbsp;- массив частот, кГц,&nbsp;</font><font size=\\\"5\\\"><math><mrow><mi><br></mi><mi>f</mi></mrow></math>.</font></li><li><font size=\\\"5\\\"><i>ft</i>&nbsp;- частота, соответствующая максимуму&nbsp;</font><font size=\\\"5\\\"><math><msub><mi>P</mi><mrow><mi mathvariant=\\\"normal\\\">т</mi></mrow></msub></math>, кГц, <math><mrow><msub><mi>f</mi><mrow><mi>т</mi></mrow></msub></mrow></math>.</font></li><li><font size=\\\"5\\\"><i>pt0</i>&nbsp;- значение этого максимума, определяемое глубиной места и плотностью судоходства, Па,&nbsp;</font><font size=\\\"5\\\"><math><msub><mi>P</mi><mrow><mi mathvariant=\\\"normal\\\">т</mi><mn>0</mn></mrow></msub></math>.</font></li></ul></div><div><font size=\\\"5\\\"><br></font></div></div><div><font size=\\\"5\\\"><br></font></div><div><font size=\\\"5\\\"><b>Выходы</b>:</font></div><div><font size=\\\"5\\\"><br></font></div><div><ul><li><font size=\\\"5\\\"><i>p0</i>&nbsp;- спектральный уровень шумов судоходства, Па,<math><mrow><msub><mi>P</mi><mrow><mn>0</mn><mi mathvariant=\\\"normal\\\">с</mi><mi mathvariant=\\\"normal\\\">у</mi><mi mathvariant=\\\"normal\\\">д</mi></mrow></msub><mrow><mo stretchy=\\\"false\\\">(</mo><mi>f</mi><mo stretchy=\\\"false\\\">)</mo></mrow></mrow></math></font></li></ul></div><div><span style=\\\"font-size: x-large;\\\"><br></span></div><br><br><font size=\\\"5\\\">Спектральный уровень шумов судохоства рассчитывается с помощью формулы:<br><br><math><mrow><mrow><msub><mi>P</mi><mrow><mn>0</mn><mi mathvariant=\\\"normal\\\">с</mi><mi mathvariant=\\\"normal\\\">у</mi><mi mathvariant=\\\"normal\\\">д</mi></mrow></msub><mrow><mo stretchy=\\\"false\\\">(</mo><mi>f</mi><mo stretchy=\\\"false\\\">)</mo></mrow></mrow><mo>=</mo><msqrt><mfrac><mrow><mn>2</mn><mo>×</mo><mmultiscripts><mi>P</mi><mrow><mi mathvariant=\\\"normal\\\">т</mi><mn>0</mn></mrow><none></none><none></none><mn>2</mn></mmultiscripts><mo>×</mo><mmultiscripts><mi>f</mi><mi mathvariant=\\\"normal\\\">т</mi><none></none><none></none><mn>3</mn></mmultiscripts><mo>×</mo><msup><mi>f</mi><mn>2</mn></msup></mrow><mrow><msup><mi>f</mi><mn>5</mn></msup><mo>+</mo><mmultiscripts><mi>f</mi><mi mathvariant=\\\"normal\\\">т</mi><none></none><none></none><mn>5</mn></mmultiscripts></mrow></mfrac></msqrt></mrow></math></font></body></html>";

    widget = new QWebView();
    widget->setMinimumWidth(300);
    widget->setHtml(test);

    QVBoxLayout* left_side_menu = new QVBoxLayout();
    left_side_menu->addWidget(label);
    left_side_menu->addWidget(line_edit);
    left_side_menu->addWidget(lineA);
    left_side_menu->addLayout(buttons_layout);
    left_side_menu->addWidget(input_label);
    left_side_menu->addWidget(input_text);
    left_side_menu->addWidget(const_label);
    left_side_menu->addWidget(const_text);
    left_side_menu->addWidget(algorithm_label);
    left_side_menu->addWidget(algorithm_text);
    left_side_menu->addWidget(output_label);
    left_side_menu->addWidget(output_text);
    left_side_menu->addWidget(link_label);
    left_side_menu->addWidget(link_text);

    QSplitter* splitter = new QSplitter();
    splitter->setChildrenCollapsible(false);
    splitter->setOrientation(Qt::Horizontal);

    QWidget* left_side_container = new QWidget();
    left_side_container->setLayout(left_side_menu);
    splitter->addWidget(left_side_container);
    splitter->addWidget(widget);
    splitter->setHandleWidth(2);
    splitter->setFrameShadow(QFrame::Sunken);

    connect(line_edit, SIGNAL(editingFinished()), SLOT(UpdatePreview()));




    tabs_ = new QTabWidget();
    tabs_->setFont(font_size_);
    tabs_->addTab(splitter, "Модуль");
    form_layout_->addWidget(tabs_);
}

void Form::UpdatePreview() {
    widget->setHtml("<!DOCTYPE html>\n"
                    "<html><head></head><body><div><div><font size=\\\"6\\\"><b>"+line_edit->text()+"</b></font></div><div><font size=\\\"5\\\"><br></font></div><div><font size=\\\"5\\\"><br></font></div><div><font size=\\\"5\\\"><b>Входы</b>:</font></div><div><font size=\\\"5\\\"><br></font></div><div><ul><li><font size=\\\"5\\\"><i>f</i>&nbsp;- массив частот, кГц,&nbsp;</font><font size=\\\"5\\\"><math><mrow><mi><br></mi><mi>f</mi></mrow></math>.</font></li><li><font size=\\\"5\\\"><i>ft</i>&nbsp;- частота, соответствующая максимуму&nbsp;</font><font size=\\\"5\\\"><math><msub><mi>P</mi><mrow><mi mathvariant=\\\"normal\\\">т</mi></mrow></msub></math>, кГц, <math><mrow><msub><mi>f</mi><mrow><mi>т</mi></mrow></msub></mrow></math>.</font></li><li><font size=\\\"5\\\"><i>pt0</i>&nbsp;- значение этого максимума, определяемое глубиной места и плотностью судоходства, Па,&nbsp;</font><font size=\\\"5\\\"><math><msub><mi>P</mi><mrow><mi mathvariant=\\\"normal\\\">т</mi><mn>0</mn></mrow></msub></math>.</font></li></ul></div><div><font size=\\\"5\\\"><br></font></div></div><div><font size=\\\"5\\\"><br></font></div><div><font size=\\\"5\\\"><b>Выходы</b>:</font></div><div><font size=\\\"5\\\"><br></font></div><div><ul><li><font size=\\\"5\\\"><i>p0</i>&nbsp;- спектральный уровень шумов судоходства, Па,<math><mrow><msub><mi>P</mi><mrow><mn>0</mn><mi mathvariant=\\\"normal\\\">с</mi><mi mathvariant=\\\"normal\\\">у</mi><mi mathvariant=\\\"normal\\\">д</mi></mrow></msub><mrow><mo stretchy=\\\"false\\\">(</mo><mi>f</mi><mo stretchy=\\\"false\\\">)</mo></mrow></mrow></math></font></li></ul></div><div><span style=\\\"font-size: x-large;\\\"><br></span></div><br><br><font size=\\\"5\\\">Спектральный уровень шумов судохоства рассчитывается с помощью формулы:<br><br><math><mrow><mrow><msub><mi>P</mi><mrow><mn>0</mn><mi mathvariant=\\\"normal\\\">с</mi><mi mathvariant=\\\"normal\\\">у</mi><mi mathvariant=\\\"normal\\\">д</mi></mrow></msub><mrow><mo stretchy=\\\"false\\\">(</mo><mi>f</mi><mo stretchy=\\\"false\\\">)</mo></mrow></mrow><mo>=</mo><msqrt><mfrac><mrow><mn>2</mn><mo>×</mo><mmultiscripts><mi>P</mi><mrow><mi mathvariant=\\\"normal\\\">т</mi><mn>0</mn></mrow><none></none><none></none><mn>2</mn></mmultiscripts><mo>×</mo><mmultiscripts><mi>f</mi><mi mathvariant=\\\"normal\\\">т</mi><none></none><none></none><mn>3</mn></mmultiscripts><mo>×</mo><msup><mi>f</mi><mn>2</mn></msup></mrow><mrow><msup><mi>f</mi><mn>5</mn></msup><mo>+</mo><mmultiscripts><mi>f</mi><mi mathvariant=\\\"normal\\\">т</mi><none></none><none></none><mn>5</mn></mmultiscripts></mrow></mfrac></msqrt></mrow></math></font></body></html>");
}