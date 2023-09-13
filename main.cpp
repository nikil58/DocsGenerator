#include <QApplication>
#include "Form.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyleSheet("QSplitter::handle { margin: 15px; background-color: gray }");
    Form mainWindow;
    return QApplication::exec();
}
