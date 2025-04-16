#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDateTime>
#include <QCoreApplication>
#include <QDebug>

#include "schedule.h"

class dbManager
{
private:
    dbManager();
    ~dbManager();

    dbManager(const dbManager&) = delete;
    dbManager& operator=(const dbManager&) = delete;

    bool initDb(); // db 초기화 함수
    bool createScheduleTable(); // 테이블이 없을 때 테이블을 생성하는 함수

    bool isDateInSchedule(const QDate& date, const QDate& start, const QDate& end); // 어떤 날짜가 일정 안에 포함되는지 확인하는 함수
    bool isDateInRepeatedSchedule(const QDate& date, const QDate& sTime, const QDate& eTime, const QString& period); // 어떤 날짜가 반복되는 일정 안에 포함되는지 확인하는 함수

    QString dbName;
    QString dbPath;
    QSqlDatabase db;

    static dbManager* instancePtr;

public:
    static dbManager& instance(); // singleton instance access

    int insertSchedule(const Schedule& s); // 삽입 함수
    bool deleteSchedule(const Schedule& s); // 삭제 함수
    bool modifySchedule(const Schedule& s); // 수정 함수
    QList<Schedule> searchSchedule(const QDateTime &start, const QDateTime &end); // 조회 함수
    QList<Schedule> getSchedulesForDate(const QDate &date); // 어떤 날짜의 일정을 조회 및 반환하는 함수
    QList<Schedule> searchScheduleName(const QString& name); // 일정 이름으로 조회하는 함수
};

#endif // DBMANAGER_H
