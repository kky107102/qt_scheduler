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

    bool initDb();
    bool createScheduleTable();
    bool isDateInSchedule(const QDateTime& date, const QDateTime& start, const QDateTime& end);

    QString dbName;
    QString dbPath;
    QSqlDatabase db;

    static dbManager* instancePtr;

public:
    static dbManager& instance(); // singleton instance access

    int insertSchedule(const Schedule& s);
    bool deleteSchedule(const Schedule& s);
    bool modifySchedule(const Schedule& s);
    QList<Schedule> searchSchedule(const QDateTime &start, const QDateTime &end);

    QList<Schedule> getSchedulesForDate(const QDateTime &date);
};

#endif // DBMANAGER_H
