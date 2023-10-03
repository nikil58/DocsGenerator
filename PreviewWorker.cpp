#include "PreviewWorker.h"
#include <QLineEdit>
#include <QTextEdit>


PreviewWorker::PreviewWorker(int mode, QObject* parent) : mode_(mode) {
    thread_ = new QThread;
    thread_->setObjectName("Preview");
    this->moveToThread(thread_);
    thread_->start();
    connect(this, SIGNAL(RenderIsReady(QString)), parent, SLOT(Rerender(QString)));
    Update();
}


void PreviewWorker::UpdatePreview() {
    if (QLineEdit* line_edit = qobject_cast<QLineEdit*>(QObject::sender())) {
        if (line_edit->objectName() == "title") {
            title_field_ = line_edit->text();
        } else if (line_edit->objectName() == "link") {
            link_field_ = line_edit->text();
        }
    } else if (QTextEdit* text_edit = qobject_cast<QTextEdit*>(QObject::sender())) {
        if (text_edit->objectName() == "inputs") {
            inputs_field_ = text_edit->toPlainText().replace("\n",
                                                             "<p class=\"western\" align=\"justify\" style=\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\">");
            inputs_field_ = Parse(inputs_field_);
        } else if (text_edit->objectName() == "constants") {
            const_field_ = text_edit->toPlainText().replace("\n",
                                                            "<p class=\"western\" align=\"justify\" style=\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\">");
            const_field_ = Parse(const_field_);
        } else if (text_edit->objectName() == "algorithm") {
            algorithm_field_ = text_edit->toPlainText().replace("\n",
                                                                "<p class=\"western\" align=\"justify\" style=\"text-align: center; line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm;\">");
            algorithm_field_ = Parse(algorithm_field_);
        } else if (text_edit->objectName() == "outputs") {
            output_field_ = text_edit->toPlainText().replace("\n",
                                                             "<p class=\"western\" align=\"justify\" style=\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\">");
            output_field_ = Parse(output_field_);
        }

    }

    Update();
}

void PreviewWorker::Update() {
    //qDebug() << QThread::currentThread()->objectName();
    QString title_start = "<html><head></head><body><h1><font face=\"Times New Roman, serif\"><span style=\"font-size: 16px;\">";
    QString title = title_field_.replace("\n", "<br>");
    QString title_end = "</span></font></h1><font style=\"font-size: 12pt\"><font color=\"#000000\"><font face=\"Times New Roman, serif\">";
    QString inputs_start = "";
    QString inputs = "";
    QString inputs_end = "";
    if (!inputs_field_.isEmpty() && inputs_field_.count(" ") != inputs_field_.length()) {
        inputs_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                       "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\">Вход:</u>"
                       "    </p>"
                       "<p class=\"western\" align=\"justify\" style=\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\">";
        inputs = inputs_field_;
        inputs_end = "</p>";
    }

    QString const_start = "";
    QString const_field = "";
    QString const_end = "";
    if (!const_field_.isEmpty() && const_field_.count(" ") != const_field_.length()) {
        const_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                      "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\">Константы:</u>"
                      "    </p>\n"
                      "    <p class=\"western\" align=\"justify\" style=\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\">";
        const_field = const_field_;
        const_end = "</p>";
    }

    QString algorithm_start = "";
    QString algorithm = "";
    QString algorithm_end = "";
    if (!algorithm_field_.isEmpty() && algorithm_field_.count(" ") != algorithm_field_.length()) {
        algorithm_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                          "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\">Алгоритм:</u>"
                          "    </p>\n"
                          "    <p class=\"western\" align=\"justify\" style=\"text-align: center; line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm;\">";
        algorithm = algorithm_field_;
        algorithm_end = "</p>";
    }

    QString output_start = "";
    QString output = "";
    QString output_end = "";
    if (!output_field_.isEmpty() && output_field_.count(" ") != output_field_.length()) {
        output_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\">"
                       "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\">Выход:</u>\n"
                       "    </p>\n"
                       "    <p class=\"western\" align=\"justify\" style=\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\">";
        output = output_field_;
        output_end = "</p>";
    }

    QString link_start = "";
    QString link = "";
    QString link_end = "";
    if (!link_field_.isEmpty() && link_field_.count(" ") != link_field_.length()) {
        link_start = "<p class=\"western\" align=\"justify\" style=\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\">\n"
                     "        <u style=\"font-family: 'Times New Roman', serif; font-size: 16px;\"><a href=modelica://";
        link = link_field_.replace("\n", "<br>");
        link_end = ">Пример использования</a></u>\n"
                           "    </p>";
    }

    text_ = title_start + title + title_end + inputs_start + inputs + inputs_end + const_start + const_field +
            const_end + algorithm_start + algorithm + algorithm_end + output_start + output + output_end + link_start +
            link + link_end;
    emit RenderIsReady(text_);
}

QString PreviewWorker::Parse(QString text) {
    QString result = text;
    if (text.contains("\\(") && text.contains("\\)")) {
        QString first_part = text.split("\\(")[0];
        QStringRef last_part_ref(&text, text.indexOf("\\)") + 2, text.length() - text.indexOf("\\)") - 2);
        QString last_part = last_part_ref.toString();
        QStringRef substring(&text, text.indexOf("\\("), text.indexOf("\\)") + 2 - text.indexOf("\\("));
        if (formulas_cache_.contains(substring.toString())) {
            result = formulas_cache_.value(substring.toString());
        }
        QFile file("./temp.txt");
        if (formulas_cache_.contains(substring.toString()) != true && file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << substring.toString();
            file.close();
            QProcess* proc = new QProcess();
            proc->start("node ./latex_to_mathml.js ./temp.txt");
            proc->waitForFinished();
            result = proc->readAllStandardOutput();
            file.remove();
            proc->terminate();
            formulas_cache_.insert(substring.toString(), result);
        }
        if (last_part.contains("\\(") && last_part.contains("\\)")) {
            last_part = Parse(last_part);
        }
        result = first_part + result;
        result.replace("\n", "</math>" + last_part);
    }
    //qDebug() << result;
    return result;
}

PreviewWorker::~PreviewWorker() noexcept {}

void PreviewWorker::ClearCache() {
    formulas_cache_.clear();
}