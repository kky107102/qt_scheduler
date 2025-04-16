#include "dbmanager.h"

dbManager* dbManager::instancePtr = nullptr;

dbManager::dbManager()
{
    initDb();
}
dbManager::~dbManager()
{
    if (db.isOpen())
    {
        db.close();
    }
}

dbManager& dbManager::instance()
{
    if (instancePtr == nullptr)
    {
        instancePtr = new dbManager();
    }

    return *instancePtr;
}

bool dbManager::initDb()
{
    dbName = "scheduleDB";
    dbPath = QCoreApplication::applicationDirPath() + "/schedule.db";
    db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    db.setDatabaseName(dbPath);

    qDebug() << "[DEBUG] DB Path:" << dbPath;

    if (!db.open())
    {
        qDebug() << "[ERROR] Database open failed: " << db.lastError().text();
        //QSqlDatabase::removeDatabase(dbName);
        QMessageBox::critical(nullptr, "오류", "데이터베이스를 찾을 수 없습니다.");
        return false;
    }

    // tbl 이미 있는지 확인
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM sqlite_master WHERE name='scheduleTbl'");
    if (!query.exec()) // tbl 없음
    {
        qDebug() << "[ERROR] query.exec() failed";
        return false;
    }

    if (query.next())
    {
        int ret = query.value(0).toInt();
        if (ret == 0)
            createScheduleTable();
    }

    return true;
}

bool dbManager::isDateInSchedule(const QDate& date, const QDate& start, const QDate& end)
{
    if (date >= start && date <= end)
        return true;
    else
        return false;
}

bool dbManager::isDateInRepeatedSchedule(const QDate& date, const QDate& sTime, const QDate& eTime, const QString& period)
{
    if (date < sTime) return false;

    if (period == "1주 마다")
    {
        int daysBetween = sTime.daysTo(date);
        if (daysBetween < 0) return false;

        int repeatCycle = 7;
        int repetition = daysBetween / repeatCycle;

        QDate shiftedStart = sTime.addDays(repetition * repeatCycle);
        QDate shiftedEnd = eTime.addDays(repetition * repeatCycle);

        return (date >= shiftedStart && date <= shiftedEnd);
    }
    else if (period == "1개월 마다")
    {
        int monthsBetween = (date.year() - sTime.year()) * 12 + (date.month() - sTime.month());
        if (monthsBetween < 0) return false;

        QDate shiftedStart = sTime.addMonths(monthsBetween);
        QDate shiftedEnd = eTime.addMonths(monthsBetween);

        return (date >= shiftedStart && date <= shiftedEnd);
    }

    return false;
}

bool dbManager::createScheduleTable()
{
    QSqlQuery query(db);
    query.prepare("CREATE TABLE scheduleTbl ("
                  "id INTEGER PRIMARY KEY, "
                  "name VARCHAR(100), "
                  "start DATETIME, "
                  "end DATETIME, "
                  "location VARCHAR(100), "
                  "memo VARCHAR(200), "
                  "period VARCHAR(10))");

    if(!query.exec())
    {
        qDebug() << "[ERROR] createtbl query.exec() failed";
        return false;
    }

    qDebug() << "[DEBUG] success to create scheduleTbl";

    return true;
}

int dbManager::insertSchedule(const Schedule& s)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO scheduleTbl (name, start, end, location, memo, period) "
                  "VALUES (:name, :start, :end, :location, :memo, :period)");
    query.bindValue(":name", s.getScheduleName());
    query.bindValue(":start", s.getStartTime());
    query.bindValue(":end", s.getEndTime());
    query.bindValue(":location", s.getLocation());
    query.bindValue(":memo", s.getMemo());
    query.bindValue(":period", s.getPeriod());
    if(!query.exec())
    {
        qDebug() << "[ERROR] insert query.exec() failed:" << query.lastError().text();
        return -1; // fail
    }

    int insertedId = query.lastInsertId().toInt();
    qDebug() << "[DEBUG] inserted Id: " << insertedId;

    return insertedId;
}

bool dbManager::modifySchedule(const Schedule& s)
{
    QSqlQuery query(db);
    query.prepare("UPDATE scheduleTbl SET "
                  "name = :name, "
                  "start = :start, "
                  "end = :end, "
                  "location = :location, "
                  "memo = :memo, "
                  "period = :period "
                  "WHERE id = :id");
    query.bindValue(":name", s.getScheduleName());
    query.bindValue(":start", s.getStartTime());
    query.bindValue(":end", s.getEndTime());
    query.bindValue(":location", s.getLocation());
    query.bindValue(":memo", s.getMemo());
    query.bindValue(":id", s.getScheduleId());
    query.bindValue(":period", s.getPeriod());

    if (!query.exec()) {
        qDebug() << "[ERROR] Failed to modify schedule: " << query.lastError().text();
        return false;
    }

    return true;
}

bool dbManager::deleteSchedule(const Schedule& s)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM scheduleTbl WHERE id = :id");
    query.bindValue(":id", s.getScheduleId());

    if (!query.exec()) {
        qDebug() << "[ERROR] Failed to delete schedule: " << query.lastError().text();
        return false;
    }

    return true;
}

QList<Schedule> dbManager::searchSchedule(const QDateTime &start, const QDateTime &end)
{
    QList<Schedule> scheduleList;

    QSqlQuery query(db);
    //query.prepare("SELECT * FROM scheduleTbl WHERE start >= :start AND end <= :end");
    query.prepare("SELECT * FROM scheduleTbl");
    query.bindValue(":start", start);
    query.bindValue(":end", end);

    if (!query.exec())
    {
        qDebug() << "[ERROR] failed to search schedule";
        return scheduleList;
    }

    while (query.next())
    {
        Schedule s;
        s.setScheduleId(query.value("id").toInt());
        s.setScheduleName(query.value("name").toString());
        s.setStartTime(query.value("start").toDateTime());
        s.setEndTime(query.value("end").toDateTime());
        s.setLocation(query.value("location").toString());
        s.setMemo(query.value("memo").toString());
        s.setPeriod(query.value("period").toString());

        scheduleList.append(s);
    }

    return scheduleList;
}

QList<Schedule> dbManager::getSchedulesForDate(const QDate& date)
{
    QList<Schedule> sList;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM scheduleTbl");
    if (!query.exec())
    {
        qDebug() << "[ERROR] failed to getschedulesfordate query.exec()";
        return sList;
    }

    while(query.next())
    {   
        Schedule s;
        s.setScheduleId(query.value("id").toInt());
        s.setScheduleName(query.value("name").toString());
        s.setStartTime(query.value("start").toDateTime());
        s.setEndTime(query.value("end").toDateTime());
        s.setLocation(query.value("location").toString());
        s.setMemo(query.value("memo").toString());
        s.setPeriod(query.value("period").toString());

        QDate sTime = s.getStartTime().date();
        QDate eTime = s.getEndTime().date();
        QString period = s.getPeriod();

        if (isDateInSchedule(date, sTime, eTime)) // 현재 날짜에 일정이 있는 경우
        {
            sList.append(s);
        }
        else if (isDateInRepeatedSchedule(date, sTime, eTime, period)) // 현재 날짜가 반복되는 일정 중 하나인 경우
        {
            sList.append(s);
        }
    }

    return sList;
}

QList<Schedule> dbManager::searchScheduleName(const QString& name)
{
    QList<Schedule> sList;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM scheduleTbl "
                  "WHERE name = :name");

    query.bindValue(":name", name);

    if (!query.exec())
    {
        qDebug() << "[ERROR] failed to searchschedulename query.exec()";
        return sList;
    }

    while (query.next())
    {
        Schedule s;
        s.setScheduleId(query.value("id").toInt());
        s.setScheduleName(query.value("name").toString());
        s.setStartTime(query.value("start").toDateTime());
        s.setEndTime(query.value("end").toDateTime());
        s.setLocation(query.value("location").toString());
        s.setMemo(query.value("memo").toString());
        s.setPeriod(query.value("period").toString());

        sList.append(s);
    }

    return sList;
}
