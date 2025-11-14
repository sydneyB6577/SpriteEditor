#include "mainwindow.h"
#include "saveandopen.h"
#include <QApplication>

// Main class that runs the whole application.
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SaveAndOpen saveAndOpen;
    MainWindow w(&saveAndOpen);
    w.show();
    return a.exec();

    qInstallMessageHandler([](QtMsgType, const QMessageLogContext &, const QString &msg)
    {
        qDebug() << msg;
    });
}
