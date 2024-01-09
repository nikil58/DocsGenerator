#pragma once

#include <QWidget>
#include <QVBoxLayout>

class CharacterForm : public QWidget{
    Q_OBJECT
private:
    QFont font_size_;
    QVBoxLayout* global_layout_;

    const QVector<QString> unary_binary_operations_ = {"PlusMinus", "MinusPlus", "cdot", "times", "frac", "div", "bigoplus", "bigominus", "bigodot", "bigotimes", "neg", "lor", "land"};
    const QVector<QString> relations_ = {"neq" , "leq", "geq", "ll", "gg", "approx", "sim", "simeq", "equiv", "perp", "parallel", "mid"};
    const QVector<QString> set_operations_ = {"in", "notin", "ni", "cap", "cup","subset", "supset", "subseteq", "supseteq", "notsubset", "notsupset", "mathbb"};
    const QVector<QString> functions_ = {"abs", "root", "sup", "sub", "cos", "sin", "tan", "arccos", "arcsin", "arctan"};
    const QVector<QString> operators_ = {"int", "sum", "prod", "lim"};
    const QVector<QString> attributes_ = {"dot", "overline", "overrightarrow", "widetilde"};
    const QVector<QString> letters_ = {"alpha", "beta", "gamma", "delta", "Delta", "epsilon", "varepsilon", "zeta", "eta", "theta", "vartheta", "Theta", "iota", "lamda", "Lambda", "mu"};
public:
    explicit CharacterForm(QWidget* parent = nullptr);
};

