#include <iostream>
#include <QApplication>
#include "Form.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Form mainWindow;
    return QApplication::exec();
}
