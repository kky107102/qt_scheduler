#include "schedule.h"

/*
    Schedule();
    Schedule(const QString &name, const QDateTime &start, const QDateTime &end,
             const QString &location, const QString &memo);
    ~Schedule();

    // getter
    int getScheduleId() const;
    QString getScheduleName() const;
    QDateTime getStartTime() const;
    QDateTime getEndTime() const;
    QString getLocation() const;
    QString getMemo() const;

    // setter
    void setScheduleId(const int&);
    void setScheduleName(const QString& name);
    void setStartTime(const QDateTime& start);
    void setEndTime(const QDateTime& end);
    void setLocation(const QString& location);
    void setMemo(const QString& memo);
*/

Schedule::Schedule() {}
Schedule::Schedule(const QString &name, const QDateTime &start, const QDateTime &end,
                   const QString &location, const QString &memo)
    : scheduleName(name), startTime(start), endTime(end), location(location), memo(memo) {}
Schedule::Schedule(const Schedule& s)
{
    this->scheduleId = s.getScheduleId();
    this->scheduleName = s.getScheduleName();
    this->startTime = s.getStartTime();
    this->endTime = s.getEndTime();
    this->location = s.getLocation();
    this->memo = s.getMemo();
}

Schedule::~Schedule() {}

// getter
int Schedule::getScheduleId() const
{
    return scheduleId;
}

QString Schedule::getScheduleName() const
{
    return scheduleName;
}

QDateTime Schedule::getStartTime() const
{
    return startTime;
}

QDateTime Schedule::getEndTime() const
{
    return endTime;
}

QString Schedule::getLocation() const
{
    return location;
}

QString Schedule::getMemo() const
{
    return memo;
}

// setter
void Schedule::setScheduleId(const int& id)
{
    scheduleId = id;
}
void Schedule::setScheduleName(const QString& name)
{
    scheduleName = name;
}
void Schedule::setStartTime(const QDateTime& start)
{
    startTime = start;
}
void Schedule::setEndTime(const QDateTime& end)
{
    endTime = end;
}
void Schedule::setLocation(const QString& location)
{
    this->location = location;
}
void Schedule::setMemo(const QString& memo)
{
    this->memo = memo;
}
