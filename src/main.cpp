#include <QApplication>
#include <QtDebug>

#include "settings.h"
#include "ui/mainwindow.h"
#include "singleinstance.h"
#include "util.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    SingleInstance si(PROGRAM_NAME);
    if (si.hasPreviousInstance()) {
        return EXIT_SUCCESS;
    }

    if (!si.start()) {
        qDebug() << si.getLastErrorString();
        return EXIT_FAILURE;
    }

    app.setApplicationDisplayName(PROGRAM_NAME);
    app.setApplicationName(PROGRAM_NAME);
    app.setApplicationVersion(Util::parseAppVersion());

    MainWindow mainWindow;
    mainWindow.show();

    QObject::connect(&si, &SingleInstance::newInstanceCreated, [&mainWindow]() {
        mainWindow.setMainWindowVisibility(true);
    });

    return app.exec();
}
