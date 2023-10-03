#include "Form.h"

int main(int argc, char *argv[]) {
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    QApplication a(argc, argv);
    a.setStyleSheet("QSplitter::handle { margin: 15px; background-color: gray }");
    Form mainWindow;
    return QApplication::exec();
}
