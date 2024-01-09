#include <QPushButton>
#include "CharacterForm.h"
#include "Spoiler.h"
#include <QDebug>

CharacterForm::CharacterForm(QWidget* parent) : QWidget(parent){
    this->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    this->setWindowTitle("Спецсимволы");
    this->setFixedSize(1280, 720);
    this->setAttribute(Qt::WA_DeleteOnClose);

    font_size_.setPixelSize(20);
    global_layout_ = new QVBoxLayout(this);
    int column = 0, row = 0;

    QGridLayout* unary_binary_layout = new QGridLayout();
    for (const auto& current_button : unary_binary_operations_) {
        QString current_button_text = current_button;
        auto* button = new QPushButton();
        button->setFont(font_size_);

        QPixmap pixmap(":"+current_button_text);
        QIcon ButtonIcon(pixmap);

        if (!ButtonIcon.isNull()) {
            button->setIcon(ButtonIcon);
            button->setIconSize(QSize(30,30));
        }
        else
            button->setText(current_button_text);
        button->setObjectName(current_button_text);
        unary_binary_layout->addWidget(button, row, column++);
        button->setMinimumHeight(40);
        if (column > 5) { /// only 5 buttons can be in a row
            column = 0;
            row++;
        }
        connect(button, SIGNAL(clicked()), parent, SLOT(OperationClick()));
    }
    auto* unary_binary_spoiler = new Spoiler("Унарные и бинарные операции");
    unary_binary_spoiler->SetContentLayout(*unary_binary_layout);

    column = 0, row = 0;
    QGridLayout* relations_layout = new QGridLayout();
    for (const auto& current_button : relations_) {
        QString current_button_text = current_button;
        auto* button = new QPushButton();
        button->setFont(font_size_);

        QPixmap pixmap(":"+current_button_text);
        QIcon ButtonIcon(pixmap);

        if (!ButtonIcon.isNull()) {
            button->setIcon(ButtonIcon);
            button->setIconSize(QSize(30,30));
        }
        else
            button->setText(current_button_text);
        button->setObjectName(current_button_text);
        relations_layout->addWidget(button, row, column++);
        button->setMinimumHeight(40);
        if (column > 5) { /// only 5 buttons can be in a row
            column = 0;
            row++;
        }
        connect(button, SIGNAL(clicked()), parent, SLOT(OperationClick()));
    }
    auto* relations_spoiler = new Spoiler("Отношения");
    relations_spoiler->SetContentLayout(*relations_layout);

    column = 0, row = 0;
    QGridLayout* set_operation_layout = new QGridLayout();
    for (const auto& current_button : set_operations_) {
        QString current_button_text = current_button;
        auto* button = new QPushButton();
        button->setFont(font_size_);

        QPixmap pixmap(":"+current_button_text);
        QIcon ButtonIcon(pixmap);

        if (!ButtonIcon.isNull()) {
            button->setIcon(ButtonIcon);
            button->setIconSize(QSize(30,30));
        }
        else
            button->setText(current_button_text);
        button->setObjectName(current_button_text);
        set_operation_layout->addWidget(button, row, column++);
        button->setMinimumHeight(40);
        if (column > 5) { /// only 5 buttons can be in a row
            column = 0;
            row++;
        }
        connect(button, SIGNAL(clicked()), parent, SLOT(OperationClick()));
    }
    auto* set_operation_spoiler = new Spoiler("Множества");
    set_operation_spoiler->SetContentLayout(*set_operation_layout);

    column = 0, row = 0;
    QGridLayout* function_layout = new QGridLayout();
    for (const auto& current_button : functions_) {
        QString current_button_text = current_button;
        auto* button = new QPushButton();
        button->setFont(font_size_);

        QPixmap pixmap(":"+current_button_text);
        QIcon ButtonIcon(pixmap);

        if (!ButtonIcon.isNull()) {
            button->setIcon(ButtonIcon);
            button->setIconSize(QSize(30,30));
        }
        else
            button->setText(current_button_text);
        button->setObjectName(current_button_text);
        function_layout->addWidget(button, row, column++);
        button->setMinimumHeight(40);
        if (column > 5) { /// only 5 buttons can be in a row
            column = 0;
            row++;
        }
        connect(button, SIGNAL(clicked()), parent, SLOT(OperationClick()));
    }
    auto* function_spoiler = new Spoiler("Функции");
    function_spoiler->SetContentLayout(*function_layout);

    column = 0, row = 0;
    QGridLayout* operators_layout = new QGridLayout();
    for (const auto& current_button : operators_) {
        QString current_button_text = current_button;
        auto* button = new QPushButton();
        button->setFont(font_size_);

        QPixmap pixmap(":"+current_button_text);
        QIcon ButtonIcon(pixmap);

        if (!ButtonIcon.isNull()) {
            button->setIcon(ButtonIcon);
            button->setIconSize(QSize(30,30));
        }
        else
            button->setText(current_button_text);
        button->setObjectName(current_button_text);
        operators_layout->addWidget(button, row, column++);
        button->setMinimumHeight(40);
        if (column > 5) { /// only 5 buttons can be in a row
            column = 0;
            row++;
        }
        connect(button, SIGNAL(clicked()), parent, SLOT(OperationClick()));
    }
    auto* operators_spoiler = new Spoiler("Операторы");
    operators_spoiler->SetContentLayout(*operators_layout);


    column = 0, row = 0;
    QGridLayout* attributes_layout = new QGridLayout();
    for (const auto& current_button : attributes_) {
        QString current_button_text = current_button;
        auto* button = new QPushButton();
        button->setFont(font_size_);

        QPixmap pixmap(":"+current_button_text);
        QIcon ButtonIcon(pixmap);

        if (!ButtonIcon.isNull()) {
            button->setIcon(ButtonIcon);
            button->setIconSize(QSize(30,30));
        }
        else
            button->setText(current_button_text);
        button->setObjectName(current_button_text);
        attributes_layout->addWidget(button, row, column++);
        button->setMinimumHeight(40);
        if (column > 5) { /// only 5 buttons can be in a row
            column = 0;
            row++;
        }
        connect(button, SIGNAL(clicked()), parent, SLOT(OperationClick()));
    }
    auto* attributes_spoiler = new Spoiler("Аттрибуты");
    attributes_spoiler->SetContentLayout(*attributes_layout);

    global_layout_->addWidget(unary_binary_spoiler);
    global_layout_->addWidget(relations_spoiler);
    global_layout_->addWidget(set_operation_spoiler);
    global_layout_->addWidget(function_spoiler);
    global_layout_->addWidget(operators_spoiler);
    global_layout_->addWidget(attributes_spoiler);
    this->show();
}