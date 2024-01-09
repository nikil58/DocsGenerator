#include <QPushButton>
#include "CharacterForm.h"
#include "Spoiler.h"
#include <QDebug>

CharacterForm::CharacterForm(QWidget* parent) : QWidget(parent){
    this->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    this->setWindowTitle("Спецсимволы");
    this->setFixedSize(400, 720);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(parent->x() - 400, parent->y() + 25, 400, 720);

    font_size_.setPixelSize(20);
    global_layout_ = new QVBoxLayout(this);

    auto* unary_binary_spoiler = new Spoiler("Унарные и бинарные операции");
    unary_binary_spoiler->SetContentLayout(*PlaceButton(unary_binary_operations_));

    auto* relations_spoiler = new Spoiler("Отношения");
    relations_spoiler->SetContentLayout(*PlaceButton(relations_));

    auto* set_operation_spoiler = new Spoiler("Множества");
    set_operation_spoiler->SetContentLayout(*PlaceButton(set_operations_));

    auto* function_spoiler = new Spoiler("Функции");
    function_spoiler->SetContentLayout(*PlaceButton(functions_));

    auto* operators_spoiler = new Spoiler("Операторы");
    operators_spoiler->SetContentLayout(*PlaceButton(operators_));

    auto* attributes_spoiler = new Spoiler("Аттрибуты");
    attributes_spoiler->SetContentLayout(*PlaceButton(attributes_));

    auto* splitters_layout = new QVBoxLayout();
    splitters_layout->addWidget(unary_binary_spoiler);
    splitters_layout->addWidget(relations_spoiler);
    splitters_layout->addWidget(set_operation_spoiler);
    splitters_layout->addWidget(function_spoiler);
    splitters_layout->addWidget(operators_spoiler);
    splitters_layout->addWidget(attributes_spoiler);

    auto* container = new QWidget();
    container->setLayout(splitters_layout);
    auto* scroll_area = new QScrollArea();
    scroll_area->setStyleSheet("QScrollArea {background-color: transparent;}");
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(container);

    for (int i = 0; i < splitters_layout->count(); ++i) {
        qobject_cast<Spoiler*>(splitters_layout->itemAt(i)->widget())->ToggleButton();
        qobject_cast<Spoiler*>(splitters_layout->itemAt(i)->widget())->ToggleButton();
    }
    qobject_cast<Spoiler*>(splitters_layout->itemAt(0)->widget())->ToggleButton();

    global_layout_->addWidget(scroll_area);

    this->show();
}

QGridLayout* CharacterForm::PlaceButton(const QVector<QString>& buttons) {
    QGridLayout* layout = new QGridLayout();
    int column = 0, row = 0;
    for (const auto& current_button : buttons) {
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
        layout->addWidget(button, row, column++);
        button->setMinimumHeight(40);
        if (column > 2) { /// only 3 buttons can be in a row
            column = 0;
            row++;
        }
        connect(button, SIGNAL(clicked()), this->parent(), SLOT(OperationClick()));
    }
    return layout;
}