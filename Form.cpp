#include "Form.h"

#include <QLabel>
#include <QPushButton>
#include <QWebFrame>
#include <QClipboard>
#include <QSplitter>

Form::Form() {
    QThread::currentThread()->setObjectName("Main");
    this->setWindowFlags(Qt::Window/* | Qt::WindowStaysOnTopHint*/);
    this->setWindowTitle("Генератор документации");
    this->setMinimumSize(1280, 900);

    font_size_.setPixelSize(20);
    form_layout_ = new QVBoxLayout(this);
    DrawMainForm();

    this->show();
}

void Form::DrawMainForm() {
    title_field_ = new QLineEdit();
    title_field_->setFont(font_size_);
    title_field_->setObjectName("title");

    QLabel* label = new QLabel("Название");
    label->setFont(font_size_);

    auto under_line = new QFrame();
    under_line->setFrameShape(QFrame::HLine);
    under_line->setFrameShadow(QFrame::Sunken);

    QGridLayout* buttons_layout = new QGridLayout();
    int column = 0, row = 0;
    for (const auto& current_button_text : formulas_buttons_) {
        QPushButton* button = new QPushButton(current_button_text);
        button->setFont(font_size_);
        button->setObjectName(current_button_text);
        buttons_layout->addWidget(button,row, column++);
        button->setMinimumHeight(40);
        if (column > 5) {
            column = 0;
            row++;
        }
        connect(button, SIGNAL(clicked()), this, SLOT(OperationClick()));
    }

    QLabel* input_label = new QLabel("Входы");
    input_label->setFont(font_size_);
    inputs_field_ = new QTextEdit();
    inputs_field_->setFont(font_size_);
    inputs_field_->setObjectName("inputs");
    input_label->setBuddy(inputs_field_);

    QLabel* const_label = new QLabel("Константы");
    const_label->setFont(font_size_);
    const_field_ = new QTextEdit();
    const_field_->setFont(font_size_);
    const_field_->setObjectName("constants");
    const_label->setBuddy(const_field_);

    QLabel* algorithm_label = new QLabel("Алгоритм");
    algorithm_label->setFont(font_size_);
    algorithm_field_ = new QTextEdit();
    algorithm_field_->setFont(font_size_);
    algorithm_field_->setObjectName("algorithm");
    algorithm_label->setBuddy(algorithm_field_);

    QLabel* output_label = new QLabel("Выход");
    output_label->setFont(font_size_);
    output_field_ = new QTextEdit();
    output_field_->setFont(font_size_);
    output_field_->setObjectName("outputs");
    output_label->setBuddy(output_field_);

    QLabel* link_label = new QLabel("Ссылка на пример");
    link_label->setFont(font_size_);
    link_field_ = new QLineEdit();
    link_field_->setFont(font_size_);
    link_field_->setObjectName("link");
    link_label->setBuddy(link_field_);

    QPushButton* copy_button = new QPushButton("Копировать");
    copy_button->setFont(font_size_);
    connect(copy_button, SIGNAL(clicked()), this, SLOT(CopyButtonClicked()));

    preview_widget_ = new QWebView();
    preview_widget_->setMinimumWidth(300);
    preview_widget_->setDisabled(true);
    PreviewWorker* worker = new PreviewWorker(0, this);

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

    QVBoxLayout* preview_button_layout = new QVBoxLayout();
    preview_button_layout->addWidget(copy_button);
    preview_button_layout->addWidget(preview_widget_);
    QWidget* right_container = new QWidget();
    right_container->setLayout(preview_button_layout);

    QWidget* left_side_container = new QWidget();
    left_side_container->setLayout(left_side_menu);
    splitter->addWidget(left_side_container);
    splitter->addWidget(right_container);
    splitter->setHandleWidth(2);
    splitter->setFrameShadow(QFrame::Sunken);

    connect(title_field_, SIGNAL(textChanged(const QString &)), worker, SLOT(UpdatePreview()));
    connect(inputs_field_, SIGNAL(textChanged()), worker, SLOT(UpdatePreview()));
    connect(const_field_, SIGNAL(textChanged()), worker, SLOT(UpdatePreview()));
    connect(algorithm_field_, SIGNAL(textChanged()), worker, SLOT(UpdatePreview()));
    connect(output_field_, SIGNAL(textChanged()), worker, SLOT(UpdatePreview()));
    connect(link_field_, SIGNAL(textChanged(const QString &)), worker, SLOT(UpdatePreview()));

    inputs_field_->installEventFilter(this);
    const_field_->installEventFilter(this);
    algorithm_field_->installEventFilter(this);
    output_field_->installEventFilter(this);

    tabs_ = new QTabWidget();
    tabs_->setFont(font_size_);
    tabs_->addTab(splitter, "Модуль");
    form_layout_->addWidget(tabs_);
}


void Form::OperationClick() {
    if (last_selected_field_) {
        if (QObject::sender()->objectName() == formulas_buttons_[0]) {
            last_selected_field_->insertPlainText("\\(\\)");
            last_selected_field_->moveCursor(QTextCursor::Left);
            last_selected_field_->moveCursor(QTextCursor::Left);
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[1]){
            last_selected_field_->insertPlainText("_{}");
            last_selected_field_->moveCursor(QTextCursor::Left);
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[2]) {
            last_selected_field_->insertPlainText("^{}");
            last_selected_field_->moveCursor(QTextCursor::Left);
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[3]) {
            last_selected_field_->insertPlainText("\\sqrt{}");
            last_selected_field_->moveCursor(QTextCursor::Left);
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[4]) {
            last_selected_field_->insertPlainText("\\frac{}{}");
            last_selected_field_->moveCursor(QTextCursor::Left);
            last_selected_field_->moveCursor(QTextCursor::Left);
            last_selected_field_->moveCursor(QTextCursor::Left);
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[5]) {
            last_selected_field_->insertPlainText("\\Delta");
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[6]) {
            last_selected_field_->insertPlainText("\\sum{}");
            last_selected_field_->moveCursor(QTextCursor::Left);
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[7]) {
            last_selected_field_->insertPlainText("\\prod{}");
            last_selected_field_->moveCursor(QTextCursor::Left);
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[8]) {
            last_selected_field_->insertPlainText("\\int");
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[9]) {
            last_selected_field_->insertPlainText("\\times");
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[10]) {
            last_selected_field_->insertPlainText("\\pi");
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[11]) {
            last_selected_field_->insertPlainText("\\leq");
            last_selected_field_->setFocus();
        }
        else if (QObject::sender()->objectName() == formulas_buttons_[12]) {
            last_selected_field_->insertPlainText("\\infty");
            last_selected_field_->setFocus();
        }
    }

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

    QFile file("./temp.html");
    if (file.open(QIODevice::ReadWrite)){
        QTextStream stream(&file);
        stream << preview_widget_->page()->mainFrame()->toHtml();
        file.close();
        QProcess* proc = new QProcess();
        proc->start("tidy -i -m -w 160 ./temp.html");
        proc->waitForFinished();
        file.open(QIODevice::ReadOnly);
        QString result = stream.readAll().remove("<!DOCTYPE html>\n");
        clipboard->setText(result, QClipboard::Clipboard);
        if (clipboard->supportsSelection()) {
            clipboard->setText(result, QClipboard::Selection);
        }
#if defined (Q_OS_LINUX)
        QThread::msleep(1);
#endif
        file.close();
        file.remove();
        proc->terminate();
    }
}

void Form::Rerender(QString text) {
    preview_widget_->setHtml(text);
}