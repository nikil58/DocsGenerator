#include "PreviewWorker.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QFile>


PreviewWorker::PreviewWorker(int mode, QObject* parent) : mode_(mode){
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
        }
        else if (line_edit->objectName() == "link"){
            link_field_ = line_edit->text();
        }
    }
    else if (QTextEdit* text_edit = qobject_cast<QTextEdit*>(QObject::sender())){
        if (text_edit->objectName() == "inputs")
            inputs_field_ = text_edit->toPlainText();
        else if (text_edit->objectName() == "constants")
            const_field_ = text_edit->toPlainText();
        else if (text_edit->objectName() == "algorithm")
            algorithm_field_ = text_edit->toPlainText();
        else if (text_edit->objectName() == "outputs")
            output_field_ = text_edit->toPlainText();
    }

    Update();
}

void PreviewWorker::Update() {
    //qDebug() << QThread::currentThread()->objectName();
    QString title_start = "<html><head></head><body><h1><font face=\\\"Times New Roman, serif\\\"><span style=\\\"font-size: 16px;\\\">";
    QString title = title_field_.replace("\n", "<br>");
    QString title_end = "</span></font></h1>";
    QString inputs_start = "<p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                           "        <u style=\\\"font-family: 'Times New Roman', serif; font-size: 16px;\\\">Вход:</u>\n"
                           "    </p>"
                           "<p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                           "    <font face=\\\"Liberation Serif, serif\\\">\n"
                           "        <font style=\\\"font-size: 12pt\\\"><font color=\\\"#000000\\\"><font face=\\\"Times New Roman, serif\\\">\n"
                           "        <i>";
    QString inputs = inputs_field_.replace("\n", "<br>");
    inputs = Parse(inputs);
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
    QString const_field = const_field_.replace("\n", "<br>");
    QString const_end = "</sub></i>\n"
                        "    </font>\n"
                        "    </font></font></font></p>";
    QString algorithm_start = "<p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                              "        <u style=\\\"font-family: 'Times New Roman', serif; font-size: 16px;\\\">Алгоритм:</u>\n"
                              "    </p>\n"
                              "    <p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                              "    <font face=\\\"Liberation Serif, serif\\\">\n"
                              "    <p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"text-align: center; line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm;\\\"><font face=\\\"Liberation Serif, serif\\\"><font style=\\\"font-size: 12pt\\\"><font color=\\\"#000000\\\"><font face=\\\"Times New Roman, serif\\\">";
    QString algorithm = algorithm_field_.replace("\n", "<br>");
    QString algorithm_end = "</font></font></font></font></p>";
    QString output_start = "<p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                           "        <u style=\\\"font-family: 'Times New Roman', serif; font-size: 16px;\\\">Выход:</u>\n"
                           "    </p>\n"
                           "    <p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                           "    <font face=\\\"Liberation Serif, serif\\\">\n"
                           "    <font style=\\\"font-size: 12pt\\\"><font color=\\\"#000000\\\"><font face=\\\"Times New Roman, serif\\\">\n"
                           "        <i>";
    QString output = output_field_.replace("\n", "<br>");
    QString output_end = "</i>\n"
                         "    </font>\n"
                         "    </font></font></font></p>";
    QString link_start = "<p class=\\\"western\\\" align=\\\"justify\\\" style=\\\"line-height: 115%; text-indent: 1.25cm; margin-bottom: 0cm\\\">\n"
                         "        <u style=\\\"font-family: 'Times New Roman', serif; font-size: 16px;\\\"><a href=\\";
    QString link = link_field_.replace("\n", "<br>");
    QString link_end = "\\\">Пример использования</a></u>\n"
                       "    </p>";

    text_ = title_start + title + title_end + inputs_start + inputs + inputs_end + const_start + const_field + const_end + algorithm_start + algorithm + algorithm_end + output_start +output + output_end + link_start + link + link_end;
    emit RenderIsReady(text_);
}

QString PreviewWorker::Parse(QString text) {
    QString result = text;
    if (text.contains("\\(")&&text.contains("\\)")){
        QString first_part = text.split("\\(")[0];
        QStringRef last_part(&text, text.indexOf("\\)") + 2, text.length() - text.indexOf("\\)") - 2);
        QString test = last_part.toString();
        QStringRef substring(&text, text.indexOf("\\("), text.indexOf("\\)") + 2 - text.indexOf("\\("));
        QFile file("./temp.txt");
        if (file.open(QIODevice::ReadWrite)){
            QTextStream stream(&file);
            stream << substring.toString();
            file.close();
            QProcess* proc = new QProcess();
            proc->start("node ./latex_to_mathml.js ./temp.txt");
            proc->waitForFinished();
            result = proc->readAllStandardOutput();
            file.remove();
            proc->terminate();

        }
        if (last_part.contains("\\(")&&last_part.contains("\\)")) {
            test = Parse(last_part.toString());
        }
        result = first_part + result;
        result.replace("\n", "</math>"+test);

    }
    qDebug() << result;
    return result;
}

PreviewWorker::~PreviewWorker() noexcept {}