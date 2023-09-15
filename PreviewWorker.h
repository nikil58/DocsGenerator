#pragma once

#include <QDebug>
#include <QThread>
#include <QProcess>

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
    QString link_field_;

    QString text_{};


    void Update();
    QString Parse(QString text);
public:
    explicit PreviewWorker(int mode, QObject* parent = nullptr);
    ~PreviewWorker();
    signals:
    void RenderIsReady(QString text);
public slots:
    void UpdatePreview();
};

