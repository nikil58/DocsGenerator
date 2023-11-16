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

    /// Mode: 0 - for module, 1 - for model
    int mode_;
    /**
     * Below are fields that used to fill the html doc
     * <br>
     * START
     */
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
    // END

    /// Common Text without script
    QString text_{};

    /// Every cached LaTeX formula
    QMap<QString, QString> formulas_cache_{};

    /// Method that generate HTML for module
    void FirstTypeForm();

    /// Method that generate HTML for model
    void SecondTypeForm();

    /// Method that invokes when new key in inputs appears
    void Update();

    /**
     * @brief Recoursive method that check all inside \(\) and replace it by the MathML
     * @param text with LaTeX in \(\)
     * @return text with MathML
     */
    QString Parse(const QString& text);
public:
    explicit PreviewWorker(int mode, QObject* parent = nullptr);
    ~PreviewWorker() override;
    /**
     * @brief Setter for mode fieild that can be useful when tabs switch
     * @param mode
     */
    void SetMode(int mode) { mode_ = mode; Update();};
signals:
    /// Signals that emits in the end of the update
    void RenderIsReady(QString text);
public slots:
    /// Slot that clear QMap of formulas
    void ClearCache();
    /// Slot that get all inputs from form to local fields
    void UpdatePreview();
};

