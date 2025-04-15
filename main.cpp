#include "mainwindow.h"

#include <QApplication>
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dbManager& db = dbManager::instance();
    MainWindow w;
    w.resize(600, 500);
    w.show();
    return a.exec();
}
