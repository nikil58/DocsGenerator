#include "Form.h"
#include "Spoiler.h"

#include <QPushButton>
#include <QClipboard>
#include <QMenuBar>
#include <QFileDialog>
#include <QSettings>
#include <utility>

Form::Form() {
    QThread::currentThread()->setObjectName("Main");
    this->setWindowFlags(Qt::Window/* | Qt::WindowStaysOnTopHint*/);
    this->setWindowTitle("Генератор документации");
    this->setMinimumSize(1280, 900);

    font_size_.setPixelSize(20);
    form_layout_ = new QHBoxLayout();
    auto* widget = new QWidget;
    widget->setLayout(form_layout_);
    DrawMainForm();

    auto* import_menu = new QAction("&Импорт", this);
    auto* export_menu = new QAction("&Экспорт", this);
    QMenu* file = menuBar()->addMenu("&Файл");
    menuBar()->setFont(font_size_);
    file->setFont(font_size_);
    file->addAction(import_menu);
    file->addAction(export_menu);
    connect(import_menu, SIGNAL(triggered(bool)), this, SLOT(ImportFile(bool)));
    connect(export_menu, SIGNAL(triggered(bool)), this, SLOT(ExportFile(bool)));

    this->setCentralWidget(widget);
    this->show();
}

void Form::DrawMainForm() {
    title_field_ = new QLineEdit();
    title_field_->setFont(font_size_);
    title_field_->setObjectName("title");

    title_label_ = new QLabel("Название");
    title_label_->setFont(font_size_);

    title_splitter_ = new QFrame();
    title_splitter_->setFrameShape(QFrame::HLine);
    title_splitter_->setFrameShadow(QFrame::Sunken);

    formulas_buttons_layout_ = new QGridLayout();
    int column = 0, row = 0;
    for (const auto& current_button : formulas_buttons_) {
        QString current_button_text = current_button.at(0);
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
        formulas_buttons_layout_->addWidget(button, row, column++);
        button->setMinimumHeight(40);
        if (column > 5) { /// only 5 buttons can be in a row
            column = 0;
            row++;
        }
        connect(button, SIGNAL(clicked()), this, SLOT(OperationClick()));
    }

    auto* copy_button = new QPushButton("Копировать");
    copy_button->setFont(font_size_);
    connect(copy_button, SIGNAL(clicked()), this, SLOT(CopyButtonClicked()));

    auto* clear_button = new QPushButton("Очистить");
    clear_button->setFont(font_size_);
    connect(clear_button, SIGNAL(clicked()), this, SLOT(ClearButtonClicked()));

    preview_widget_ = new QWebEngineView();
    preview_widget_->setMinimumWidth(300);
    connect(preview_widget_, SIGNAL(urlChanged(const QUrl &)), this, SLOT(ClickOnLink(const QUrl &))); /// Do now allow link clicks
    worker_ = new PreviewWorker(0, this);
    connect(this, SIGNAL(ClearCache()), worker_, SLOT(ClearCache()));
    connect(title_field_, SIGNAL(textChanged(const QString &)),  worker_, SLOT(UpdatePreview()));

    buttons_layout_ = new QHBoxLayout();
    buttons_layout_->addWidget(copy_button);
    buttons_layout_->addWidget(clear_button);

    tabs_ = new QTabWidget();
    connect(tabs_, SIGNAL(currentChanged(int)), this, SLOT(SwitchTab(int)));
    tabs_->setFont(font_size_);
    tabs_->addTab(DrawFirstTab(), "Модуль");
    tabs_->addTab(DrawSecondTab(), "Модель");

    auto* left_side_layout = new QVBoxLayout();
    left_side_layout->addWidget(title_label_);
    left_side_layout->addWidget(title_field_);
    left_side_layout->addWidget(title_splitter_);
    left_side_layout->addLayout(formulas_buttons_layout_);
    left_side_layout->addWidget(tabs_);

    auto* preview_button_layout = new QVBoxLayout();
    preview_button_layout->addLayout(buttons_layout_);
    preview_button_layout->addWidget(preview_widget_);
    auto* right_side_container = new QWidget();
    right_side_container->setLayout(preview_button_layout);

    auto* splitter = new QSplitter(); /// vertical line that allows change size of left and right parts
    splitter->setChildrenCollapsible(false);
    splitter->setOrientation(Qt::Horizontal);

    auto* container = new QWidget();
    container->setLayout(left_side_layout);

    splitter->addWidget(container);
    splitter->addWidget(right_side_container);
    splitter->setHandleWidth(2);
    splitter->setFrameShadow(QFrame::Sunken);
    splitter->setSizes(QList<int>({1,1}));

    form_layout_->addWidget(splitter);
}

QWidget* Form::DrawFirstTab() {
    auto* input_label = new QLabel("Входы");
    input_label->setFont(font_size_);
    inputs_field_ = new QTextEdit();
    inputs_field_->setFont(font_size_);
    inputs_field_->setObjectName("inputs");
    inputs_field_->setAcceptRichText(false);
    input_label->setBuddy(inputs_field_);
    /*QVBoxLayout* test = new QVBoxLayout();
    test->addWidget(input_label);
    test->addWidget(inputs_field_);

    Spoiler* spoiler = new Spoiler();
    spoiler->SetContentLayout(*test);*/

    auto* const_label = new QLabel("Константы");
    const_label->setFont(font_size_);
    const_field_ = new QTextEdit();
    const_field_->setFont(font_size_);
    const_field_->setObjectName("constants");
    const_field_->setAcceptRichText(false);
    const_label->setBuddy(const_field_);

    auto* algorithm_label = new QLabel("Алгоритм");
    algorithm_label->setFont(font_size_);
    algorithm_field_ = new QTextEdit();
    algorithm_field_->setFont(font_size_);
    algorithm_field_->setObjectName("algorithm");
    algorithm_field_->setAcceptRichText(false);
    algorithm_label->setBuddy(algorithm_field_);

    auto* output_label = new QLabel("Выход");
    output_label->setFont(font_size_);
    output_field_ = new QTextEdit();
    output_field_->setFont(font_size_);
    output_field_->setObjectName("outputs");
    output_field_->setAcceptRichText(false);
    output_label->setBuddy(output_field_);

    auto* link_label = new QLabel("Ссылка на пример");
    link_label->setFont(font_size_);
    link_field_1_ = new QLineEdit();
    link_field_1_->setFont(font_size_);
    link_field_1_->setObjectName("link");
    link_label->setBuddy(link_field_1_);

    auto* left_side_container = new QWidget();
    auto* left_side_menu = new QVBoxLayout(left_side_container);
    left_side_menu->addWidget(input_label);
    left_side_menu->addWidget(inputs_field_);
//    left_side_menu->addWidget(spoiler);
    left_side_menu->addWidget(const_label);
    left_side_menu->addWidget(const_field_);
    left_side_menu->addWidget(algorithm_label);
    left_side_menu->addWidget(algorithm_field_);
    left_side_menu->addWidget(output_label);
    left_side_menu->addWidget(output_field_);
    left_side_menu->addWidget(link_label);
    left_side_menu->addWidget(link_field_1_);


    auto* scroll_area = new QScrollArea();
    scroll_area->setStyleSheet("QScrollArea {background-color: transparent;}");
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(left_side_container);

    connect(inputs_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(const_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(algorithm_field_, SIGNAL(textChanged()), worker_, SLOT(UpdatePreview()));
    connect(output_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(link_field_1_, SIGNAL(textChanged(const QString &)),  worker_, SLOT(UpdatePreview()));

    inputs_field_->installEventFilter(this);
    const_field_->installEventFilter(this);
    algorithm_field_->installEventFilter(this);
    output_field_->installEventFilter(this);

    return scroll_area;
}

QWidget* Form::DrawSecondTab() {
    auto* input_description = new QLabel("Описание входов");
    input_description->setFont(font_size_);
    input_description_field_ = new QTextEdit();
    input_description_field_->setFont(font_size_);
    input_description_field_->setObjectName("inputs_description");
    input_description_field_->setAcceptRichText(false);
    input_description->setBuddy(input_description_field_);

    auto* input_list = new QLabel("Список входов");
    input_list->setFont(font_size_);
    input_list_field_ = new QTextEdit();
    input_list_field_->setFont(font_size_);
    input_list_field_->setObjectName("inputs_list");
    input_list_field_->setAcceptRichText(false);
    input_list->setBuddy(input_list_field_);

    auto* output_description = new QLabel("Описание выходов");
    output_description->setFont(font_size_);
    output_description_field_ = new QTextEdit();
    output_description_field_->setFont(font_size_);
    output_description_field_->setObjectName("output_description");
    output_description_field_->setAcceptRichText(false);
    output_description->setBuddy(output_description_field_);

    auto* output_list = new QLabel("Список выходов");
    output_list->setFont(font_size_);
    output_list_field_ = new QTextEdit();
    output_list_field_->setFont(font_size_);
    output_list_field_->setObjectName("output_list");
    output_list_field_->setAcceptRichText(false);
    output_list->setBuddy(output_list_field_);

    auto* link_label = new QLabel("Ссылка на модуль");
    link_label->setFont(font_size_);
    link_field_2_ = new QLineEdit();
    link_field_2_->setFont(font_size_);
    link_field_2_->setObjectName("link");
    link_label->setBuddy(link_field_2_);

    auto* section_name_label = new QLabel("Название дополнительного раздела");
    section_name_label->setFont(font_size_);
    section_name_ = new QLineEdit();
    section_name_->setFont(font_size_);
    section_name_->setObjectName("section_name");
    section_name_label->setBuddy(section_name_);

    auto* section_field_label = new QLabel("Содержание");
    section_field_label->setFont(font_size_);
    section_field_ = new QTextEdit();
    section_field_->setFont(font_size_);
    section_field_->setObjectName("section_field");
    section_field_->setAcceptRichText(false);
    section_field_label->setBuddy(section_field_);

    auto* left_side_container = new QWidget();
    auto* left_side_menu = new QVBoxLayout(left_side_container);
    left_side_menu->addWidget(input_description);
    left_side_menu->addWidget(input_description_field_);
    left_side_menu->addWidget(input_list);
    left_side_menu->addWidget(input_list_field_);
    left_side_menu->addWidget(output_description);
    left_side_menu->addWidget(output_description_field_);
    left_side_menu->addWidget(output_list);
    left_side_menu->addWidget(output_list_field_);
    left_side_menu->addWidget(link_label);
    left_side_menu->addWidget(link_field_2_);
    left_side_menu->addWidget(section_name_label);
    left_side_menu->addWidget(section_name_);
    left_side_menu->addWidget(section_field_label);
    left_side_menu->addWidget(section_field_);

    auto* scroll_area = new QScrollArea();
    scroll_area->setStyleSheet("QScrollArea {background-color: transparent;}");
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(left_side_container);

    connect(input_description_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(input_list_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(output_description_field_, SIGNAL(textChanged()), worker_, SLOT(UpdatePreview()));
    connect(output_list_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(link_field_2_, SIGNAL(textChanged(const QString &)),  worker_, SLOT(UpdatePreview()));
    connect(section_name_, SIGNAL(textChanged(const QString &)),  worker_, SLOT(UpdatePreview()));
    connect(section_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));

    input_description_field_->installEventFilter(this);
    input_list_field_->installEventFilter(this);
    output_description_field_->installEventFilter(this);
    output_list_field_->installEventFilter(this);

    return scroll_area;
}


void Form::OperationClick() {
    if (last_selected_field_) {
        QString button_text = QObject::sender()->objectName();
        for (const auto& button : formulas_buttons_) {
            if (button.at(0) == button_text) {
                if (!button.at(1).isEmpty())
                    AddSymbolToField(button.at(1), button.at(2));
                else {
                    QString photo_path = QFileDialog::getOpenFileName(this, tr("Выбрать"), QDir::currentPath(), tr("Image (PNG, JPG, JPEG, TIFF) (*.png *.jpg *.jpeg *.tiff) ;; All files (*.*)"));
                    if (!photo_path.isEmpty())
                        last_selected_field_->insertPlainText("!img("+photo_path+")");
                }
                last_selected_field_->setFocus();
                break;
            }
        }
    }
}

void Form::AddSymbolToField(const QString& symbol, const QString& shift_count) {
    last_selected_field_->insertPlainText(symbol);
    for (int i = 0; i < shift_count.toInt(); ++i)
        last_selected_field_->moveCursor(QTextCursor::Left);
}

bool Form::eventFilter(QObject* obj, QEvent* e) {
    if (e->type() == QEvent::FocusOut) {
        auto* text_edit = qobject_cast<QTextEdit*>(obj);
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
        stream << text_in_preview_;
        file.close();
        auto* proc = new QProcess();
        proc->start("tidy", QStringList() << "-i" << "-m" << "-w 160" << "./temp.html"); /// tidy is for beatify html
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
        delete proc;
    }
}


void Form::ClearButtonClicked() {
    title_field_->clear();
    inputs_field_->clear();
    const_field_->clear();
    algorithm_field_->clear();
    output_field_->clear();
    link_field_1_->clear();

    input_description_field_->clear();
    input_list_field_->clear();
    output_description_field_->clear();
    output_list_field_->clear();
    link_field_2_->clear();
    section_name_->clear();
    section_field_->clear();

    emit ClearCache();
}

void Form::Rerender(QString text) {
    preview_widget_->load(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/index.html")); /// have to use load instead of "setHtml" because JavaScript won't change string but file - will
    text_in_preview_ = std::move(text);
}

void Form::SwitchTab(int index) {
    if (index == 0) {
        worker_->SetMode(0);
    }
    else if (index == 1) {
        worker_->SetMode(1);
    }
}

void Form::ClickOnLink(const QUrl& url) {
    if (url.toString().contains("modelica"))
        inputs_field_->textChanged();
}

void Form::ImportFile(bool) {
    QString file_name = QFileDialog::getOpenFileName(this, tr("Открыть"), QDir::currentPath(), tr("Config file (*.ini) ;; All files (*)"));

    auto* settings = new QSettings(file_name, QSettings::IniFormat);
    title_field_->setText(settings->value("title").toString());
    inputs_field_->setText(settings->value("inputs_field").toString());
    const_field_->setText(settings->value("const_field").toString());
    algorithm_field_->setText(settings->value("algorithm_field").toString());
    output_field_->setText(settings->value("output_field").toString());
    link_field_1_->setText(settings->value("link_field_1").toString());
    input_description_field_->setText(settings->value("input_description_field").toString());
    input_list_field_->setText(settings->value("input_list_field").toString());
    output_description_field_->setText(settings->value("output_description_field").toString());
    output_list_field_->setText(settings->value("output_list_field").toString());
    link_field_2_->setText(settings->value("link_field_2").toString());
    section_name_->setText(settings->value("section_name").toString());
    section_field_->setText(settings->value("section_field").toString());

    delete settings;
}

void Form::ExportFile(bool) {
    QString file_name = QFileDialog::getSaveFileName(this, tr("Сохранить в"), QDir::currentPath(), tr("Config file (*.ini) ;; All files (*)"));
    if (!file_name.contains(".ini"))
        file_name += ".ini";

    auto* settings = new QSettings(file_name, QSettings::IniFormat);
    settings->setValue("title", title_field_->text());
    settings->setValue("inputs_field",inputs_field_->toPlainText());
    settings->setValue("const_field",const_field_->toPlainText());
    settings->setValue("algorithm_field",algorithm_field_->toPlainText());
    settings->setValue("output_field",output_field_->toPlainText());
    settings->setValue("link_field_1",link_field_1_->text());
    settings->setValue("input_description_field",input_description_field_->toPlainText());
    settings->setValue("input_list_field",input_list_field_->toPlainText());
    settings->setValue("output_description_field",output_description_field_->toPlainText());
    settings->setValue("output_list_field",output_list_field_->toPlainText());
    settings->setValue("link_field_2",link_field_2_->text());
    settings->setValue("section_name",section_name_->text());
    settings->setValue("section_field",section_field_->toPlainText());

    delete settings;
}

Form::~Form() {
    delete worker_;
    delete this->centralWidget();
    QFile file("./index.html");
    file.remove();
}
