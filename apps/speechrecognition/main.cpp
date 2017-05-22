#include "application.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    //qputenv("QT_SCALE_FACTOR", "1");

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

      Application app(argc, argv);

      app.showMainWindow();

      return app.exec();
}
