#include "mainwindow.h"

#include <QApplication>
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int id = QFontDatabase::addApplicationFont(":/resource/NanumJungHagSaeng.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);

    QFont customFont(family);
    customFont.setPointSize(14); // 원하는 사이즈
    QApplication::setFont(customFont);
    dbManager& db = dbManager::instance();
    MainWindow w;
    w.resize(600, 500);
    w.show();
    return a.exec();
}
