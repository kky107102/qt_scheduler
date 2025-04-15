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

bool dbManager::isDateInSchedule(const QDateTime& date, const QDateTime& start, const QDateTime& end)
{
    if (date >= start && date <= end)
        return true;
    else
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
                  "memo VARCHAR(200))");

    if(!query.exec())
    {
        qDebug() << "[ERROR] query.exec() failed";
        return false;
    }

    qDebug() << "[DEBUG] success to create scheduleTbl";

    return true;
}

int dbManager::insertSchedule(const Schedule& s)
{
    QSqlQuery query(db);
    query.prepare("insert into scheduleTbl (name, start, end, location, memo) "
                  "values (:name, :start, :end, :location, :memo)");
    query.bindValue(":name", s.getScheduleName());
    query.bindValue(":start", s.getStartTime());
    query.bindValue(":end", s.getEndTime());
    query.bindValue(":location", s.getLocation());
    query.bindValue(":memo", s.getMemo());
    if(!query.exec())
    {
        qDebug() << "[ERROR] query.exec() failed";
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
                  "memo = :memo "
                  "WHERE id = :id");
    query.bindValue(":name", s.getScheduleName());
    query.bindValue(":start", s.getStartTime());
    query.bindValue(":end", s.getEndTime());
    query.bindValue(":location", s.getLocation());
    query.bindValue(":memo", s.getMemo());
    query.bindValue(":id", s.getScheduleId());

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
    query.prepare("SELECT * FROM scheduleTbl WHERE start >= :start AND end <= :end");
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

        scheduleList.append(s);
    }

    return scheduleList;
}

QList<Schedule> dbManager::getSchedulesForDate(const QDateTime &date)
{
    QList<Schedule> sList;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM scheduleTbl");
    if (!query.exec())
    {
        qDebug() << "[ERROR] failed to query.exec()";
        return sList;
    }

    while(query.next())
    {
        QDateTime sTime = query.value("start").toDateTime();
        QDateTime eTime = query.value("end").toDateTime();

        if (isDateInSchedule(date, sTime, eTime)) {
            Schedule s;
            s.setScheduleId(query.value("id").toInt());
            s.setScheduleName(query.value("name").toString());
            s.setStartTime(query.value("start").toDateTime());
            s.setEndTime(query.value("end").toDateTime());
            s.setLocation(query.value("location").toString());
            s.setMemo(query.value("memo").toString());
            sList.append(s);
        }
    }

    return sList;
}
