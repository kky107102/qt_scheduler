#include "mainwindow.h"

#include <QApplication>
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dbManager& db = dbManager::instance();

    // INSERT
    Schedule s("회의", QDateTime::currentDateTime(),
               QDateTime::currentDateTime().addSecs(3600),
               "회의실 A", "중요한 회의입니다.");

    int newId = db.insertSchedule(s);
    qDebug() << "[TEST] Inserted ID:" << newId;

    // UPDATE
    s.setScheduleId(newId); // id 설정
    s = Schedule("변경된 회의", QDateTime::currentDateTime(),
                 QDateTime::currentDateTime().addSecs(7200),
                 "회의실 B", "시간 연장됨.");
    s.setScheduleId(newId); // 수정할 ID 유지
    db.modifySchedule(s);
    qDebug() << "[TEST] Modified schedule with ID:" << newId;

    // DELETE
    //db.deleteSchedule(s);
    //qDebug() << "[TEST] Deleted schedule with ID:" << newId;

    MainWindow w;
    w.show();
    return a.exec();
}
