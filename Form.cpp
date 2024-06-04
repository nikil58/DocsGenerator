#include "Form.h"
#include "Spoiler.h"
#include "CharacterForm.h"

#include <QPushButton>
#include <QClipboard>
#include <QMenuBar>
#include <QFileDialog>
#include <QSettings>
#include <utility>
#include <QShortcut>
#include <QMessageBox>
#include <QDirIterator>
#include <QInputDialog>

QString Form::etalon_image_path_{};

Form::Form() {
    QThread::currentThread()->setObjectName("Main");
    this->setWindowFlags(Qt::Window/* | Qt::WindowStaysOnTopHint*/);
    etalon_image_path_ = FindEtalonImagePath();
    UpdateTitle();
    this->setMinimumSize(1280, 900);

    font_size_.setPixelSize(20);
    form_layout_ = new QHBoxLayout();
    auto* widget = new QWidget;
    widget->setLayout(form_layout_);
    DrawMainForm();

    auto* import_menu = new QAction("&Импорт", this);
    auto* export_ini_menu = new QAction("&Экспорт ini", this);
    auto* export_csv_menu = new QAction("&Экспорт csv", this);
    QMenu* file = menuBar()->addMenu("&Файл");
    menuBar()->setFont(font_size_);
    file->setFont(font_size_);
    file->addAction(import_menu);
    file->addAction(export_ini_menu);
    file->addAction(export_csv_menu);
    connect(import_menu, SIGNAL(triggered(bool)), this, SLOT(ImportFile(bool)));
    connect(export_ini_menu, SIGNAL(triggered(bool)), this, SLOT(ExportIniFile(bool)));
    connect(export_csv_menu, SIGNAL(triggered(bool)), this, SLOT(ExportCSVFile(bool)));
    SetConnections();

    this->setCentralWidget(widget);
    this->show();
}

void Form::DrawMainForm() {
    title_field_ = new QLineEdit();
    title_field_->setFont(font_size_);
    title_field_->setObjectName("title");
    title_field_->setPlaceholderText("Формирование сигнала шумоизлучения объекта с заданным спектральным портретом");

    title_label_ = new QLabel("Название");
    title_label_->setFont(font_size_);

    title_splitter_ = new QFrame();
    title_splitter_->setFrameShape(QFrame::HLine);
    title_splitter_->setFrameShadow(QFrame::Sunken);

    auto* window_button = new QPushButton("Спецсимволы");
    window_button->setFont(font_size_);
    connect(window_button, SIGNAL(clicked(bool)), this, SLOT(OpenFormulas(bool)));

    auto* image_button = new QPushButton("Фотография");
    image_button->setFont(font_size_);
    image_button->setObjectName("Фотография");
    connect(image_button, SIGNAL(clicked(bool)), this, SLOT(OperationClick()));

    auto* formula_button = new QPushButton("Формула");
    formula_button->setFont(font_size_);
    formula_button->setObjectName("Формула");
    connect(formula_button, SIGNAL(clicked(bool)), this, SLOT(OperationClick()));
    auto* shortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);
    connect(shortcut, SIGNAL(activated()), formula_button, SLOT(click()));

    auto* window_formula_buttons_layout = new QHBoxLayout();
    window_formula_buttons_layout->addWidget(formula_button);
    window_formula_buttons_layout->addWidget(window_button);
    window_formula_buttons_layout->addWidget(image_button);

    auto* copy_button = new QPushButton("Копировать");
    copy_button->setFont(font_size_);
    connect(copy_button, SIGNAL(clicked()), this, SLOT(CopyButtonClicked()));

    auto* clear_button = new QPushButton("Очистить");
    clear_button->setFont(font_size_);
    connect(clear_button, SIGNAL(clicked()), this, SLOT(ClearButtonClicked()));

    preview_widget_ = new QWebEngineView();
    preview_widget_->setMinimumWidth(400);
    worker_ = new PreviewWorker(0, this);

    buttons_layout_ = new QHBoxLayout();
    buttons_layout_->addWidget(copy_button);
    buttons_layout_->addWidget(clear_button);

    tabs_ = new QTabWidget();
    tabs_->setFont(font_size_);
    tabs_->addTab(DrawFirstTab(), "Модуль");
    tabs_->addTab(DrawSecondTab(), "Модель");

    auto* left_side_layout = new QVBoxLayout();
    left_side_layout->addWidget(title_label_);
    left_side_layout->addWidget(title_field_);
    left_side_layout->addWidget(title_splitter_);
    left_side_layout->addLayout(window_formula_buttons_layout);
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
    splitter->setSizes(QList<int>({container->maximumWidth(), right_side_container->minimumWidth()}));

    form_layout_->addWidget(splitter);
}

void Form::OpenFormulas(bool) {
    if (!character_form_)
        character_form_ = new CharacterForm(this);
    else
        character_form_->showMaximized();
}

QWidget* Form::DrawFirstTab() {
    inputs_field_ = new QTextEdit();
    inputs_field_->setFont(font_size_);
    inputs_field_->setObjectName("inputs");
    inputs_field_->setAcceptRichText(false);
    inputs_field_->setPlaceholderText("\\(X^{2}-\\) квадрат икса \n\\(Y_{i}-\\) итый игрик \n\\(X + Y -\\) сумма икса и игрик");
    auto* inputs_layout = new QVBoxLayout();
    inputs_layout->addWidget(inputs_field_);
    auto* inputs_spoiler = new Spoiler("Входы", form_layout_->parentWidget());
    inputs_spoiler->SetContentLayout(*inputs_layout);

    const_field_ = new QTextEdit();
    const_field_->setFont(font_size_);
    const_field_->setObjectName("constants");
    const_field_->setPlaceholderText("\\(V_{t}-\\) текущая скорость \n\\(P_{0}-\\) уровень шумности \n\\(N -\\) число объектов");
    const_field_->setAcceptRichText(false);
    auto* const_layout = new QVBoxLayout();
    const_layout->addWidget(const_field_);
    auto* const_spoiler = new Spoiler("Константы", form_layout_->parentWidget());
    const_spoiler->SetContentLayout(*const_layout);

    output_field_ = new QTextEdit();
    output_field_->setFont(font_size_);
    output_field_->setObjectName("outputs");
    output_field_->setAcceptRichText(false);
    output_field_->setPlaceholderText("\\(L-\\) размер базы \n\\(K_{L}-\\) угол между направлениями \n\\(R -\\) радиус объекта");
    auto* output_layout = new QVBoxLayout();
    output_layout->addWidget(output_field_);
    auto* output_spoiler = new Spoiler("Выходы", form_layout_->parentWidget());
    output_spoiler->SetContentLayout(*output_layout);

    algorithm_field_ = new QTextEdit();
    algorithm_field_->setFont(font_size_);
    algorithm_field_->setObjectName("algorithm");
    algorithm_field_->setAcceptRichText(false);
    algorithm_field_->setPlaceholderText("Радиус циркуляции для модели движения объекта при выборе элемента движения с циркуляцией рассчитывается по интерполированной зависимости:\n"
                                         "\\(r_{ц} = r_{fi} + K_{R}\\cdot v\\)");
    auto* algorithm_layout = new QVBoxLayout();
    algorithm_layout->addWidget(algorithm_field_);
    auto* algorithm_spoiler = new Spoiler("Алгоритм", form_layout_->parentWidget());
    algorithm_spoiler->SetContentLayout(*algorithm_layout);

    link_field_1_ = new QLineEdit();
    link_field_1_->setFont(font_size_);
    link_field_1_->setObjectName("link");
    link_field_1_->setPlaceholderText("Etalon.Modules.EtalonExamples.ImpactSimulation.ObjectMotionParameters.RadiusCirculationExample");
    auto* link_layout = new QVBoxLayout();
    link_layout->addWidget(link_field_1_);
    auto* link_spoiler = new Spoiler("Ссылка на пример", form_layout_->parentWidget());
    link_spoiler->SetContentLayout(*link_layout);

    auto* left_side_container = new QWidget();
    auto* left_side_menu = new QVBoxLayout(left_side_container);
    left_side_menu->addWidget(inputs_spoiler);
    left_side_menu->addWidget(const_spoiler);
    left_side_menu->addWidget(output_spoiler);
    left_side_menu->addWidget(algorithm_spoiler);
    left_side_menu->addWidget(link_spoiler);
    for (int i = 0; i < left_side_menu->count(); ++i) {
        qobject_cast<Spoiler*>(left_side_menu->itemAt(i)->widget())->ToggleButton();
        qobject_cast<Spoiler*>(left_side_menu->itemAt(i)->widget())->ToggleButton();
    }
    qobject_cast<Spoiler*>(left_side_menu->itemAt(0)->widget())->ToggleButton();
    auto* scroll_area = new QScrollArea();
    scroll_area->setStyleSheet("QScrollArea {background-color: transparent;}");
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(left_side_container);

    return scroll_area;
}

QWidget* Form::DrawSecondTab() {
    input_description_field_ = new QTextEdit();
    input_description_field_->setFont(font_size_);
    input_description_field_->setObjectName("inputs_description");
    input_description_field_->setAcceptRichText(false);
    input_description_field_->setPlaceholderText("Входными данными для обработки является одна из двух матриц [3x3].\n"
                                                 "После обработки одной матрицы, на вход обработки поступает другая.");
    auto* inputs_description_layout = new QVBoxLayout();
    inputs_description_layout->addWidget(input_description_field_);
    auto* inputs_description_spoiler = new Spoiler("Описание входов", form_layout_->parentWidget());
    inputs_description_spoiler->SetContentLayout(*inputs_description_layout);

    input_list_field_ = new QTextEdit();
    input_list_field_->setFont(font_size_);
    input_list_field_->setObjectName("inputs_list");
    input_list_field_->setAcceptRichText(false);
    input_list_field_->setPlaceholderText("\\({first\\_mat}-\\) первая матрица \n\\({second\\_mat}-\\) вторая матрица");
    auto* inputs_list_layout = new QVBoxLayout();
    inputs_list_layout->addWidget(input_list_field_);
    auto* inputs_list_spoiler = new Spoiler("Список входов", form_layout_->parentWidget());
    inputs_list_spoiler->SetContentLayout(*inputs_list_layout);

    output_description_field_ = new QTextEdit();
    output_description_field_->setFont(font_size_);
    output_description_field_->setObjectName("output_description");
    output_description_field_->setAcceptRichText(false);
    output_description_field_->setPlaceholderText("Итоговая матрица модели будет отображена во всплывающем окне \"Обработанная матрица\"");
    auto* output_description_layout = new QVBoxLayout();
    output_description_layout->addWidget(output_description_field_);
    auto* output_description_spoiler = new Spoiler("Описание выходов", form_layout_->parentWidget());
    output_description_spoiler->SetContentLayout(*output_description_layout);

    output_list_field_ = new QTextEdit();
    output_list_field_->setFont(font_size_);
    output_list_field_->setObjectName("output_list");
    output_list_field_->setAcceptRichText(false);
    output_list_field_->setPlaceholderText("\\({output\\_mat}-\\) выходная матрица");
    auto* output_list_layout = new QVBoxLayout();
    output_list_layout->addWidget(output_list_field_);
    auto* output_list_spoiler = new Spoiler("Список выходов", form_layout_->parentWidget());
    output_list_spoiler->SetContentLayout(*output_list_layout);

    link_field_2_ = new QLineEdit();
    link_field_2_->setFont(font_size_);
    link_field_2_->setObjectName("link");
    link_field_2_->setPlaceholderText("Etalon.Models.experimental.cycle_processing.CycleProcessing");
    auto* link_layout = new QVBoxLayout();
    link_layout->addWidget(link_field_2_);
    auto* link_spoiler = new Spoiler("Ссылка на модуль", form_layout_->parentWidget());
    link_spoiler->SetContentLayout(*link_layout);

    section_name_ = new QLineEdit();
    section_name_->setFont(font_size_);
    section_name_->setObjectName("section_name");
    section_name_->setPlaceholderText("Описание модели");
    auto* section_name_layout = new QVBoxLayout();
    section_name_layout->addWidget(section_name_);
    auto* section_name_spoiler = new Spoiler("Название дополнительного раздела", form_layout_->parentWidget());
    section_name_spoiler->SetContentLayout(*section_name_layout);

    section_field_ = new QTextEdit();
    section_field_->setFont(font_size_);
    section_field_->setObjectName("section_field");
    section_field_->setAcceptRichText(false);
    section_field_->setPlaceholderText("Обработка матрицы производится при помощи составного блока cycle (CycleProcessing)");
    auto* section_content_layout = new QVBoxLayout();
    section_content_layout->addWidget(section_field_);
    auto* section_content_spoiler = new Spoiler("Содержание", form_layout_->parentWidget());
    section_content_spoiler->SetContentLayout(*section_content_layout);

    auto* left_side_container = new QWidget();
    auto* left_side_menu = new QVBoxLayout(left_side_container);
    left_side_menu->addWidget(inputs_description_spoiler);
    left_side_menu->addWidget(inputs_list_spoiler);
    left_side_menu->addWidget(output_description_spoiler);
    left_side_menu->addWidget(output_list_spoiler);
    left_side_menu->addWidget(link_spoiler);
    left_side_menu->addWidget(section_name_spoiler);
    left_side_menu->addWidget(section_content_spoiler);
    for (int i = 0; i < left_side_menu->count(); ++i) {
        qobject_cast<Spoiler*>(left_side_menu->itemAt(i)->widget())->ToggleButton();
        qobject_cast<Spoiler*>(left_side_menu->itemAt(i)->widget())->ToggleButton();
    }
    qobject_cast<Spoiler*>(left_side_menu->itemAt(0)->widget())->ToggleButton();

    auto* scroll_area = new QScrollArea();
    scroll_area->setStyleSheet("QScrollArea {background-color: transparent;}");
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(left_side_container);

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
                    QString photo_path = QFileDialog::getOpenFileName(this, tr("Выбрать"), GetLastDirectoryPath(), tr("Image (PNG, JPG, JPEG, TIFF, GIF) (*.png *.jpg *.jpeg *.tiff *.gif) ;; All files (*.*)"));
                    if (!photo_path.isEmpty()) {
                        CopyImageToEtalon(photo_path);
                    }
                }
                last_selected_field_->setFocus();
                break;
            }
        }
        this->activateWindow();
    }
}

void Form::AddSymbolToField(const QString& symbol, const QString& shift_count) {
    QTextCursor cursor(last_selected_field_->textCursor());
    const QString selected_text = cursor.selectedText();
    last_selected_field_->insertPlainText(symbol);
    for (int i = 0; i < shift_count.toInt(); ++i)
        last_selected_field_->moveCursor(QTextCursor::Left);
    last_selected_field_->insertPlainText(selected_text);
}

bool Form::eventFilter(QObject* obj, QEvent* e) {
    if (e->type() == QEvent::FocusIn) {
        auto* text_edit = qobject_cast<QTextEdit*>(obj);
        if (text_edit)
            last_selected_field_ = text_edit;
    }
    return false;
}

void Form::CopyButtonClicked(){
    bool is_empty{false};
    QStringList empty_fields;
    if (tabs_->currentIndex() == 0) {
        if (title_field_->text().isEmpty())
            empty_fields.push_back("Заголовок");
        if (inputs_field_->toPlainText().isEmpty())
            empty_fields.push_back("Входы");
        if (const_field_->toPlainText().isEmpty())
            empty_fields.push_back("Константы");
        if (output_field_->toPlainText().isEmpty())
            empty_fields.push_back("Выходы");
        if (algorithm_field_->toPlainText().isEmpty())
            empty_fields.push_back("Алгоритм");
        if (link_field_1_->text().isEmpty())
            empty_fields.push_back("Ссылка");
        is_empty = title_field_->text().isEmpty() | inputs_field_->toPlainText().isEmpty() |
                   const_field_->toPlainText().isEmpty() | algorithm_field_->toPlainText().isEmpty() |
                   output_field_->toPlainText().isEmpty() | link_field_1_->text().isEmpty();
    }
    else {
        if (title_field_->text().isEmpty())
            empty_fields.push_back("Заголовок");
        if (input_description_field_->toPlainText().isEmpty())
            empty_fields.push_back("Описание входов");
        if (input_list_field_->toPlainText().isEmpty())
            empty_fields.push_back("Список входов");
        if (output_description_field_->toPlainText().isEmpty())
            empty_fields.push_back("Описание выходов");
        if (output_list_field_->toPlainText().isEmpty())
            empty_fields.push_back("Список выходов");
        if (link_field_2_->text().isEmpty())
            empty_fields.push_back("Ссылка");
        if (!section_name_->text().isEmpty())
            if (section_field_->toPlainText().isEmpty())
            empty_fields.push_back("Содержание раздела \""+section_name_->text() + "\"");
        is_empty = title_field_->text().isEmpty() | input_description_field_->toPlainText().isEmpty() |
                input_list_field_->toPlainText().isEmpty() | output_description_field_->toPlainText().isEmpty() |
                output_list_field_->toPlainText().isEmpty() | link_field_2_->text().isEmpty();
        if (!section_name_->text().isEmpty())
            is_empty |= section_field_->toPlainText().isEmpty();
    }
    QMessageBox::StandardButton reply = QMessageBox::Yes;
    if (is_empty) {
        reply = QMessageBox::question(this, "Копировать", "Вы действительно хотите копировать текст? Некоторые поля незаполненны, это может сказаться на качестве документации.\nНезаполненные поля:\n• " + empty_fields.join("\n• "), QMessageBox::Yes | QMessageBox::No);
    }

    if (reply == QMessageBox::Yes) {
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
    else {

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
    open_file_name_.clear();
    UpdateTitle();
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

QFile Form::GetCachedFile() {
    return {QDir::homePath() + "/.etalon/DocGeneratorPath.txt"};
}

QString Form::GetLastDirectoryPath() {
    QFile file = GetCachedFile();
    QString current_dir;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        stream >> current_dir;
        return current_dir;
    }
    return QDir::currentPath();
}

void Form::SetLastDirectoryPath(QString file_path) {
    QFile file = GetCachedFile();
    file_path.remove(QFileInfo(file_path).fileName());
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << file_path;
    }
}

void Form::ImportFile(bool) {
    open_file_name_ = QFileDialog::getOpenFileName(this, tr("Открыть"), GetLastDirectoryPath(), tr("Config file (*.ini) ;; All files (*)"));
    if (open_file_name_.isEmpty())
        return;
    DisconnectIndicator();
    SetLastDirectoryPath(open_file_name_);
    auto* settings = new QSettings(open_file_name_, QSettings::IniFormat);
    title_field_->setText(settings->value("title").toString().replace(QRegExp("!img(.*/OM)"), "!img(/OM"));
    inputs_field_->setText(settings->value("inputs_field").toString().replace(QRegExp("!img(.*/OM)"), "!img(/OM"));
    const_field_->setText(settings->value("const_field").toString().replace(QRegExp("!img(.*/OM)"), "!img(/OM"));
    algorithm_field_->setText(settings->value("algorithm_field").toString().replace(QRegExp("!img(.*/OM)"), "!img(/OM"));
    output_field_->setText(settings->value("output_field").toString().replace(QRegExp("!img(.*/OM)"), "!img(/OM"));
    link_field_1_->setText(settings->value("link_field_1").toString());
    input_description_field_->setText(settings->value("input_description_field").toString().replace(QRegExp("!img(.*/OM)"), "!img(/OM"));
    input_list_field_->setText(settings->value("input_list_field").toString().replace(QRegExp("!img(.*/OM)"), "!img(/OM"));
    output_description_field_->setText(settings->value("output_description_field").toString().replace(QRegExp("!img(.*/OM)"), "!img(/OM"));
    output_list_field_->setText(settings->value("output_list_field").toString().replace(QRegExp("!img(.*/OM)"), "!img(/OM"));
    link_field_2_->setText(settings->value("link_field_2").toString());
    section_name_->setText(settings->value("section_name").toString());
    section_field_->setText(settings->value("section_field").toString().replace(QRegExp("!img(.*/OM)"), "!img(/OM"));
    ConnectIndicator();
    UpdateTitle();
    delete settings;
}

void Form::ExportIniFile(bool) {
    open_file_name_ = QFileDialog::getSaveFileName(this, tr("Сохранить в"), GetLastDirectoryPath(), tr("Config file (*.ini) ;; All files (*)"));
    if (open_file_name_.isEmpty())
        return;
    if (!open_file_name_.contains(".ini"))
        open_file_name_ += ".ini";
    SetLastDirectoryPath(open_file_name_);
    auto* settings = new QSettings(open_file_name_, QSettings::IniFormat);
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
    UpdateTitle();
    delete settings;
}

void Form::ExportCSVFile(bool) {
    open_file_name_ = QFileDialog::getSaveFileName(this, tr("Сохранить в"), GetLastDirectoryPath(),
                                                   tr("CSV File (*.csv) ;; All files (*)"));
    if (open_file_name_.isEmpty())
        return;
    if (!open_file_name_.contains(".csv"))
        open_file_name_ += ".csv";
    SetLastDirectoryPath(open_file_name_);
    QFile file(open_file_name_);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        return;
    }
    QTextStream stream(&file);

    const QRegExp regExpForCSV("\n|;|;\n");

    WriteCSV(QStringList() << "Название" << title_field_->text().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Входы" << inputs_field_->toPlainText().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Константы" << const_field_->toPlainText().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Выходы" << output_field_->toPlainText().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Алгоритм" << algorithm_field_->toPlainText().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Ссылка на пример модуля" << link_field_1_->text().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Описание входов" << input_description_field_->toPlainText().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Список входов" << input_list_field_->toPlainText().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Описание выходов" << output_description_field_->toPlainText().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Список выходов" << output_list_field_->toPlainText().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Ссылка на модуль" << link_field_2_->text().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Название дополнительного раздела" << section_name_->text().replace(regExpForCSV, "  "), stream);
    WriteCSV(QStringList() << "Содержание" << section_field_->toPlainText().replace(regExpForCSV, "  "), stream);

    file.close();
}


bool Form::WriteCSV(const QStringList& list, QTextStream& stream) {
    if(stream.status() != QTextStream::Ok || list.isEmpty()) {
        return false;
    }
    stream << list.join(";").toUtf8() << "\n";
    return true;
}

QString Form::FindEtalonImagePath() {
    QFileInfoList image_folders;
    QDirIterator iterator("/home", QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        QString filename = iterator.next();
        QFileInfo file(filename);
        if (file.isFile())
            continue;
        if (!QString::compare(file.fileName(), "Images"))
            image_folders.append(file);
    }
    for (const auto& folder: image_folders) {
        if (folder.absoluteFilePath().contains("OM/Etalon/Resources/Images"))
            return folder.absoluteFilePath();
    }
    return "";
}

void Form::CopyImageToEtalon(const QString &path) {
    if (etalon_image_path_.isEmpty()) {
        QMessageBox::critical(this, QObject::tr("Копирование"),
                              tr("Не удалось найти папку Эталона"));
        last_selected_field_->insertPlainText("!img(" + path + ")");
        return;
    }
    QString copy_path{path};
    if (!path.contains("etalon/OM/Etalon/Resources/Images")) {
        copy_path =
                etalon_image_path_ + "/" + path.split("/").last();
        if (QFile::copy(path, copy_path)) {
            QMessageBox::information(this, QObject::tr("Копирование"),
                                     tr(QString("Ваша картинка скопирована в " + copy_path).toStdString().c_str()));
        } else {
            copy_path = ProcessCopyFailure(path, copy_path, etalon_image_path_);
        }
    } else {
        QMessageBox::warning(this, QObject::tr("Копирование"),
                             tr(QString(
                                     "Ваша картинка не была скопирована автоматически, так как уже находится в этой папке: " +
                                     path).toStdString().c_str()));
    }
    if (!copy_path.isEmpty()) {
        copy_path.remove(QRegExp(".*(?=/OM)"));
        last_selected_field_->insertPlainText("!img(" + copy_path + ")");
    }
}

void Form::UpdateTitle() {
    QRegExp a("(.*/|\.ini)");
    open_file_name_.remove(a);
    QString title = "Генератор документации";
    if (!open_file_name_.isEmpty())
        title += " - ";
    this->setWindowTitle(title + open_file_name_);
}

QString Form::ProcessCopyFailure(const QString& source_path, QString destination_path, const QString& etalon_path) {
    if (!(QFile::permissions(etalon_path) & QFile::WriteUser)) {
        QMessageBox::critical(this, QObject::tr("Ошибка прав доступа"),
                              tr("Права доступа для записи в папку Images отсутствуют"));
        return source_path;
    }
    const QString replace_button_text("Заменить");
    const QString rename_button_text("Переименовать");
    QMessageBox critical_message_box(QMessageBox::Critical, QObject::tr("Ошибка копирования"),
                                     tr("Ваша картинка не была скопирована, так как в папке Images уже есть картинка с таким названием. Заменить на Вашу картинку или переименовать?"),
                     QMessageBox::Yes | QMessageBox::Cancel);
    critical_message_box.setButtonText(QMessageBox::Yes, replace_button_text);
    critical_message_box.addButton(rename_button_text, QMessageBox::ButtonRole::AcceptRole);
    critical_message_box.exec();
    if (critical_message_box.clickedButton()->text() == replace_button_text) {
        QFile::remove(destination_path);
        QFile::copy(source_path, destination_path);
        return destination_path;
    } else if (critical_message_box.clickedButton()->text() == rename_button_text) {
        bool ok;
        bool success;
        do {
            success = false;
            QString new_file_name = QInputDialog::getText(this, tr("Переименовать Файл"), tr("Новое имя файла"),
                                                  QLineEdit::Normal, destination_path.remove(QRegExp("(.*/)")), &ok);
            if (ok) {
                if (!new_file_name.isEmpty()) {
                    if (destination_path == new_file_name) {
                        QMessageBox::critical(this, QObject::tr("Ошибка"),
                                              tr("Вы не переименовали картинку"));
                        continue;
                    }
                    if (!new_file_name.contains(QRegExp("\.jpg$|\.png$|\.jpeg$|\.gif$|\.tiff$"))) {
                        QMessageBox::critical(this, QObject::tr("Ошибка"),
                                              tr("Формат картинки не поддерживается"));
                        continue;
                    }
                    if (new_file_name.contains("/") || new_file_name.contains("\\")) {
                        QMessageBox::critical(this, QObject::tr("Ошибка"),
                                              tr("Недопустимое имя картинки"));
                        continue;
                    }
                    new_file_name = etalon_path + "/" + new_file_name;
                    if (QFile::copy(source_path, new_file_name))
                        return new_file_name;
                    else {
                        QMessageBox::critical(this, QObject::tr("Ошибка"),
                                              tr("В папке Images уже есть картинка с таким названием."));
                        continue;
                    }
                } else {
                    QMessageBox::critical(this, QObject::tr("Ошибка"),
                                          tr("Вы ввели пустую строку!"));
                    continue;
                }
            }
        } while (ok && !success);
    }
    return "";
}

Form::~Form() {
    delete character_form_;
    delete worker_;
    delete this->centralWidget();
    QFile file("./index.html");
    file.remove();
}

void Form::SetChangedIndicator(){
    if (!open_file_name_.contains("*")) {
        if (open_file_name_.isEmpty())
            open_file_name_ = "Untitled";
        open_file_name_+="*";
        UpdateTitle();
    }
}

void Form::SetConnections() {
    connect(preview_widget_, SIGNAL(urlChanged(const QUrl &)), this, SLOT(ClickOnLink(const QUrl &))); /// Do now allow link clicks

    connect(tabs_, SIGNAL(currentChanged(int)), this, SLOT(SwitchTab(int)));

    connect(this, SIGNAL(ClearCache()), worker_, SLOT(ClearCache()));
    connect(title_field_, SIGNAL(textChanged(const QString &)),  worker_, SLOT(UpdatePreview()));

    connect(inputs_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(const_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(output_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(algorithm_field_, SIGNAL(textChanged()), worker_, SLOT(UpdatePreview()));
    connect(link_field_1_, SIGNAL(textChanged(const QString &)),  worker_, SLOT(UpdatePreview()));

    connect(input_description_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(input_list_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(output_description_field_, SIGNAL(textChanged()), worker_, SLOT(UpdatePreview()));
    connect(output_list_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));
    connect(link_field_2_, SIGNAL(textChanged(const QString &)),  worker_, SLOT(UpdatePreview()));
    connect(section_name_, SIGNAL(textChanged(const QString &)),  worker_, SLOT(UpdatePreview()));
    connect(section_field_, SIGNAL(textChanged()),  worker_, SLOT(UpdatePreview()));

    ConnectIndicator();

    inputs_field_->installEventFilter(this);
    const_field_->installEventFilter(this);
    algorithm_field_->installEventFilter(this);
    output_field_->installEventFilter(this);

    input_description_field_->installEventFilter(this);
    input_list_field_->installEventFilter(this);
    output_description_field_->installEventFilter(this);
    output_list_field_->installEventFilter(this);
    section_field_->installEventFilter(this);
}

void Form::DisconnectIndicator() {
    disconnect(inputs_field_, SIGNAL(textChanged()), this, SLOT(SetChangedIndicator()));
    disconnect(const_field_, SIGNAL(textChanged()), this, SLOT(SetChangedIndicator()));
    disconnect(output_field_, SIGNAL(textChanged()), this, SLOT(SetChangedIndicator()));
    disconnect(algorithm_field_, SIGNAL(textChanged()), this, SLOT(SetChangedIndicator()));
    disconnect(link_field_1_, SIGNAL(textChanged(const QString&)), this, SLOT(SetChangedIndicator()));
    disconnect(input_description_field_, SIGNAL(textChanged()), this, SLOT(SetChangedIndicator()));
    disconnect(input_list_field_, SIGNAL(textChanged()), this, SLOT(SetChangedIndicator()));
    disconnect(output_description_field_, SIGNAL(textChanged()), this, SLOT(SetChangedIndicator()));
    disconnect(output_list_field_, SIGNAL(textChanged()), this, SLOT(SetChangedIndicator()));
    disconnect(link_field_2_, SIGNAL(textChanged(const QString&)), this, SLOT(SetChangedIndicator()));
    disconnect(section_name_, SIGNAL(textChanged(const QString&)), this, SLOT(SetChangedIndicator()));
    disconnect(section_field_, SIGNAL(textChanged()), this, SLOT(SetChangedIndicator()));
}

void Form::ConnectIndicator() {
    connect(inputs_field_, SIGNAL(textChanged()),SLOT(SetChangedIndicator()));
    connect(const_field_, SIGNAL(textChanged()),SLOT(SetChangedIndicator()));
    connect(output_field_, SIGNAL(textChanged()),SLOT(SetChangedIndicator()));
    connect(algorithm_field_, SIGNAL(textChanged()),SLOT(SetChangedIndicator()));
    connect(link_field_1_, SIGNAL(textChanged(const QString &)),SLOT(SetChangedIndicator()));

    connect(input_description_field_, SIGNAL(textChanged()),SLOT(SetChangedIndicator()));
    connect(input_list_field_, SIGNAL(textChanged()),SLOT(SetChangedIndicator()));
    connect(output_description_field_, SIGNAL(textChanged()),SLOT(SetChangedIndicator()));
    connect(output_list_field_, SIGNAL(textChanged()),SLOT(SetChangedIndicator()));
    connect(link_field_2_, SIGNAL(textChanged(const QString &)),SLOT(SetChangedIndicator()));
    connect(section_name_, SIGNAL(textChanged(const QString &)),SLOT(SetChangedIndicator()));
    connect(section_field_, SIGNAL(textChanged()),SLOT(SetChangedIndicator()));
}