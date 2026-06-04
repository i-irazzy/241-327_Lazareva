#include <QApplication>
#include "mainwindow.h"
#include "httpclient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    HttpClient::instance();

    MainWindow w;
    w.show();

    return app.exec();
}
