#include "Form.h"

int main(int argc, char *argv[]) {
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1"); /// Environment variable for 4k Screens
    qputenv("QTWEBENGINE_DISABLE_SANDBOX", "1"); /// Environment variable for --no-sandbox key (it needed if use su)
    QApplication a(argc, argv);
    a.setStyleSheet("QSplitter::handle { margin: 15px; background-color: gray }");
    Form mainWindow;
    QPixmap pixmap(":icon");
    QIcon application_icon(pixmap);
    mainWindow.setWindowIcon(application_icon);
    return QApplication::exec();
}
