#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>

class CharacterForm : public QWidget{
    Q_OBJECT
private:
    QFont font_size_;
    QVBoxLayout* global_layout_;

    const QVector<QString> unary_binary_operations_ = {"pm", "mp", "cdot", "times", "frac", "div", "bigoplus", "bigominus", "bigodot", "bigotimes", "neg", "lor", "land"}; /// \usepackage{mathabx} for minus
    const QVector<QString> relations_ = {"neq" , "leq", "geq", "ll", "gg", "approx", "sim", "simeq", "equiv", "perp", "parallel", "mid"};
    const QVector<QString> set_operations_ = {"in", "notin", "ni", "cap", "cup","subset", "supset", "subseteq", "supseteq", "notsubset", "notsupset", "mathbb"}; /// \not\subset; \usepackage{amsfonts} for mathbb
    const QVector<QString> functions_ = {"|x|", "root", "sup", "sub", "cos(x)", "sin(x)", "tan(x)", "arccos(x)", "arcsin(x)", "arctan(x)"};
    const QVector<QString> operators_ = {"int", "sum", "prod", "lim"};
    const QVector<QString> attributes_ = {"dot", "overline", "overrightarrow", "widetilde"};
    const QVector<QString> letters_ = {"alpha", "beta", "gamma", "delta", "Delta", "epsilon", "varepsilon", "zeta", "eta", "theta", "vartheta", "Theta", "iota", "lambda", "Lambda", "mu", "infty"};

    QGridLayout* PlaceButton(const QVector<QString>& buttons);
public:
    explicit CharacterForm(QWidget* parent = nullptr);
};

