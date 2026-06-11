#include <QApplication>
#include <QFontDatabase>

#include "MainWindow.h"
#include "Theme.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("QtVideoDownloader");
    QApplication::setOrganizationName("Local");

    Theme::applyDark(app);

    MainWindow w;
    w.resize(1120, 720);
    w.show();

    return app.exec();
}
