#include "PreviewWorker.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QCoreApplication>
#include <QRegularExpression>


PreviewWorker::PreviewWorker(int mode, QObject* parent) : mode_(mode) {
    thread_ = new QThread;
    thread_->setObjectName("Preview");
    this->moveToThread(thread_);
    thread_->start();
    connect(this, SIGNAL(RenderIsReady(QString)), parent, SLOT(Rerender(QString)));
    Update();
}


void PreviewWorker::UpdatePreview() {
    if (auto* line_edit = qobject_cast<QLineEdit*>(QObject::sender())) {
        if (line_edit->objectName() == "title") {
            title_field_ = line_edit->text();
        } else if (line_edit->objectName() == "link") {
            if (mode_ == 0)
                link_field_1_ = line_edit->text();
            else
                link_field_2_ = line_edit->text();
        }
        else if (line_edit->objectName() == "section_name") {
            section_name_ = line_edit->text();
        }
    } else if (auto* text_edit = qobject_cast<QTextEdit*>(QObject::sender())) {
        if (text_edit->objectName() == "inputs") {
            inputs_field_ = text_edit->toPlainText().replace("\n",
                                                             R"(<p class="western" align="justify" style="line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm">)");
            inputs_field_ = Parse(inputs_field_);
        } else if (text_edit->objectName() == "constants") {
            const_field_ = text_edit->toPlainText().replace("\n",
                                                            R"(<p class="western" align="justify" style="line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm">)");
            const_field_ = Parse(const_field_);
        } else if (text_edit->objectName() == "algorithm") {
            algorithm_field_ = text_edit->toPlainText().replace("\n",
                                                                R"(<p class="western" align="justify" style="text-align: center; line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm;">)");
            algorithm_field_ = Parse(algorithm_field_);
        } else if (text_edit->objectName() == "outputs") {
            output_field_ = text_edit->toPlainText().replace("\n",
                                                             R"(<p class="western" align="justify" style="line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm">)");
            output_field_ = Parse(output_field_);
        }
        else if (text_edit->objectName() == "inputs_description") {
            inputs_description_ = text_edit->toPlainText().replace("\n", R"(<p class="western" align="justify" style="line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm">)");
            inputs_description_ = Parse(inputs_description_);
        }
        else if (text_edit->objectName() == "inputs_list") {
            inputs_list_ = "<li>" + text_edit->toPlainText().replace("\n", "<li>") + "</li>";
            inputs_list_ = Parse(inputs_list_);
        }
        else if (text_edit->objectName() == "output_description") {
            outputs_description_ = text_edit->toPlainText().replace("\n", R"(<p class="western" align="justify" style="line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm">)");
            outputs_description_ = Parse(outputs_description_);
        }
        else if (text_edit->objectName() == "output_list") {
            outputs_list_ =  "<li>" + text_edit->toPlainText().replace("\n", "<li>") + "</li>";
            outputs_list_ = Parse(outputs_list_);
        }
        else if (text_edit->objectName() == "section_field") {
            section_field_ = text_edit->toPlainText().replace("\n", R"(<p class="western" align="justify" style="line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm">)");
            section_field_ = Parse(section_field_);
        }

    }

    Update();
}

void PreviewWorker::Update() {
    //qDebug() << QThread::currentThread()->objectName();
    if (mode_ == 0)
        FirstTypeForm();
    else
        SecondTypeForm();
    emit RenderIsReady(text_);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
QString PreviewWorker::Parse(const QString& text) {
    QString result = text;
    if (text.contains("\\(")&&text.contains("\\)")){
        QString first_part = text.split("\\(")[0];
        QStringRef last_part_ref(&text, text.indexOf("\\)") + 2, text.length() - text.indexOf("\\)") - 2);
        QString last_part = last_part_ref.toString();
        QStringRef substring(&text, text.indexOf("\\("), text.indexOf("\\)") + 2 - text.indexOf("\\("));
        if (formulas_cache_.contains(substring.toString())) {
            result = formulas_cache_.value(substring.toString());
        }
        QFile file("./temp.txt");
        if (!formulas_cache_.contains(substring.toString()) && file.open(QIODevice::ReadWrite)){
            QTextStream stream(&file);
            stream << substring.toString();
            file.close();
            auto* proc = new QProcess();
            proc->start("node", QStringList() << "./latex_to_mathml.js" << "./temp.txt");
            proc->waitForFinished();
            result = proc->readAllStandardOutput();
            file.remove();
            proc->terminate();
            formulas_cache_.insert(substring.toString(),result);
        }
        if (last_part.contains("\\(")&&last_part.contains("\\)")) {
            last_part = Parse(last_part);
        }
        result = first_part + result;
        result.replace(QRegExp("alttext=\"[^\"]*\""), "");
        result.replace("\n", "</math>"+last_part);
    }
    if (result.contains("!img")) {
        QRegExp exp("!img\\(([^)]*\.(png|jpg|jpeg|gif|tiff))\\)");
        int pos = exp.indexIn(result);
        QString path = exp.cap(1);
        auto captured_text = exp.capturedTexts();
        if (!path.contains("file:") && !path.contains("modelica:"))
            path = "file:" + path;
        result = result.replace(captured_text[0], "<img src = \"" + path + "\"/>");
        result = Parse(result);
    }
    //qDebug() << result;
    return result;
}
#pragma clang diagnostic pop

PreviewWorker::~PreviewWorker() {
    thread_->quit(); /// call quit from thread
    thread_->wait(); /// wait till all things will be done
    if (thread_->isFinished())
        qDebug() << "End preview thread";
    delete thread_;
}

void PreviewWorker::ClearCache() {
    formulas_cache_.clear();
    inputs_list_="";
    outputs_list_="";
    link_field_1_="";
    link_field_2_="";
    Update();
}

void PreviewWorker::FirstTypeForm() {
    QString title_start = "<html><head>";
    QString script = R"(<script type="text/javascript" src=")" + QCoreApplication::applicationDirPath() + "/MathJax/es5/tex-mml-chtml.js\"></script>";
    QString title_start_second_part = R"(</head><body><h1><font face="Times New Roman, serif"><span style="font-size: 16px;">)";
    QString title = title_field_.replace("\n", "<br>");
    QString title_end = R"(</span></font></h1><font style="font-size: 12pt"><font color="#000000"><font face="Times New Roman, serif">)";

    QString inputs_start = "";
    QString inputs = "";
    QString inputs_end = "";
    if (!inputs_field_.isEmpty() && inputs_field_.count(" ") != inputs_field_.length()) {
        inputs_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                       "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\">Вход:</u>"
                       "    </p>"
                       "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">";
        inputs = inputs_field_;
        inputs_end = "</p>";
    }

    QString const_start = "";
    QString const_field = "";
    QString const_end = "";
    if (!const_field_.isEmpty() && const_field_.count(" ") != const_field_.length()) {
        const_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                      "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\">Константы:</u>"
                      "    </p>\n"
                      "    <p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">";
        const_field = const_field_;
        const_end = "</p>";
    }

    QString output_start = "";
    QString output = "";
    QString output_end = "";
    if (!output_field_.isEmpty() && output_field_.count(" ") != output_field_.length()) {
        output_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                       "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\">Выход:</u>\n"
                       "    </p>\n"
                       "    <p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">";
        output = output_field_;
        output_end = "</p>";
    }

    QString algorithm_start = "";
    QString algorithm = "";
    QString algorithm_end = "";
    if (!algorithm_field_.isEmpty() && algorithm_field_.count(" ") != algorithm_field_.length()) {
        algorithm_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                          "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\">Алгоритм:</u>"
                          "    </p>\n"
                          "    <p class=\"western\" align=\"justify\" style=\"text-align: center; line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm;\">";
        algorithm = algorithm_field_;
        algorithm_end = "</p>";
    }

    QString link_start = "";
    QString link = "";
    QString link_end = "</body></html>";
    if (!link_field_1_.isEmpty() && link_field_1_.count(" ") != link_field_1_.length()) {
        link_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">\n"
                     "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\"><a href=\"modelica://";
        link = link_field_1_.replace("\n", "<br>").replace("modelica://", "");
        link_end = "\">Пример использования</a></u>\n"
                   "    </p></body></html>";
    }

    QString for_file = title_start + script + title_start_second_part + title + title_end + inputs_start + inputs + inputs_end + const_start + const_field +
                       const_end + output_start + output + output_end + algorithm_start + algorithm + algorithm_end + link_start +
                       link + link_end;
    QFile file("./index.html");
    if (file.open(QIODevice::ReadWrite | QFile::Truncate)){
        QTextStream stream(&file);
        stream << for_file;
        file.close();
    }

    text_ = title_start + title_start_second_part + title + title_end + inputs_start + inputs + inputs_end + const_start + const_field +
            const_end + output_start + output + output_end + algorithm_start + algorithm + algorithm_end + link_start +
            link + link_end;
    text_.replace(QRegExp("<img src = \"file:/[^>]*OM"),"<img src = \"modelica:/");
}

void PreviewWorker::SecondTypeForm() {
    QString title_start = "<html><head>";
    QString script = R"(<script type="text/javascript" src=")" + QCoreApplication::applicationDirPath() + "/MathJax/es5/tex-mml-chtml.js\"></script>";
    QString title_start_second_part = R"(</head><body><h1><font face="Times New Roman, serif"><span style="font-size: 16px;">)";
    QString title = title_field_.replace("\n", "<br>");
    QString title_end = R"(</span></font></h1><font style="font-size: 12pt"><font color="#000000"><font face="Times New Roman, serif">)";

    QString inputs_start = "";
    QString inputs = "";
    QString inputs_end = "";
    if (!inputs_description_.isEmpty() && inputs_description_.count(" ") != inputs_description_.length()) {
        inputs_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                       "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\">Описание входов:</u>"
                       "    </p>"
                       "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">";
        inputs = inputs_description_;
        inputs_end = "</p>";
    }

    QString inputs_list_start = "";
    QString inputs_list = "";
    QString inputs_list_end = "";
    if (!inputs_list_.isEmpty() && inputs_list_.count(" ") != inputs_list_.length()) {
        inputs_list_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                            "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\"></u>"
                            "    </p>\n"
                            "  <ul>";
        inputs_list = inputs_list_;
        inputs_list_end = "</ul>";
    }

    QString outputs_start = "";
    QString outputs = "";
    QString outputs_end = "";
    if (!outputs_description_.isEmpty() && outputs_description_.count(" ") != outputs_description_.length()) {
        outputs_start =  "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                         "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\">Описание выходов:</u>"
                         "    </p>"
                         "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">";
        outputs = outputs_description_;
        outputs_end = "</p>";
    }

    QString outputs_list_start = "";
    QString outputs_list = "";
    QString outputs_list_end = "";
    if (!outputs_list_.isEmpty() && outputs_list_.count(" ") != outputs_list_.length()) {
        outputs_list_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                             "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\"></u>"
                             "    </p>\n"
                             "  <ul>";
        outputs_list = outputs_list_;
        outputs_list_end = "</ul>";
    }


    QString section_name_start = "";
    QString section_name = "";
    QString section_name_end = "";
    QString section_field = "";
    QString section_field_end = "";

    if (!section_name_.isEmpty() && section_name_.count(" ") != section_name_.length()) {
        section_name_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                             "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\">";
        section_name = section_name_;
        section_name_end = "</u>"
                               "    </p>"
                               "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">";

        if (!section_field_.isEmpty() && section_field_.count(" ") != section_field_.length()) {
            section_field = section_field_;
            section_field_end = "</p>";
        }
    }


    QString link_start = "";
    QString link="";
    QString link_end = "</body></html>";
    if (link_field_2_ != "") {
        link_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 170%; text-indent: 1.25cm; margin-bottom: 0cm\">\n"
                     "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\"><a href=\"modelica://";
        link = link_field_2_.replace("\n", "<br>").replace("modelica://", "");
        link_end = "\">Ссылка на модуль</a></u>\n"
                           "</p></body></html>";
    }
    QString for_file = text_ = title_start + script + title_start_second_part + title + title_end + inputs_start + inputs + inputs_end + inputs_list_start + inputs_list +
                               inputs_list_end + outputs_start + outputs + outputs_end + outputs_list_start + outputs_list +
                               outputs_list_end + section_name_start + section_name + section_name_end +
                               section_field + section_field_end +  link_start + link + link_end;

    QFile file("./index.html");
    if (file.open(QIODevice::ReadWrite | QFile::Truncate)){
        QTextStream stream(&file);
        stream << for_file;
        file.close();
    }

    text_ = title_start + title_start_second_part + title + title_end + inputs_start + inputs + inputs_end + inputs_list_start + inputs_list +
            inputs_list_end + outputs_start + outputs + outputs_end + outputs_list_start + outputs_list +
            outputs_list_end + section_name_start + section_name + section_name_end +
            section_field + section_field_end + link_start + link + link_end;
    text_.replace(QRegExp("<img src = \"file:/[^>]*OM"),"<img src = \"modelica:/");
}