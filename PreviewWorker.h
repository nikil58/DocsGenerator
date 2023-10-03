#pragma once

#include <QDebug>
#include <QThread>
#include <QProcess>
#include <QFile>

class PreviewWorker : public QObject{
Q_OBJECT
private:
    /// Pointer to the thread where imitator will work
    QThread* thread_;
    int mode_;


    QString title_field_;
    QString inputs_field_;
    QString const_field_;
    QString algorithm_field_;
    QString output_field_;
    QString link_field_1_;

    QString inputs_description_;
    QString inputs_list_;
    QString outputs_description_;
    QString outputs_list_;
    QString link_field_2_;
    QString section_name_;
    QString section_field_;

    QString text_{};
    QMap<QString, QString> formulas_cache_{};

    void FirstTypeForm();
    void SecondTypeForm();
    void Update();
    QString Parse(QString text);
public:
    explicit PreviewWorker(int mode, QObject* parent = nullptr);
    ~PreviewWorker();
    void SetMode(int mode) { mode_ = mode; Update();};
signals:
    void RenderIsReady(QString text);
public slots:
    void ClearCache();
    void UpdatePreview();
};

