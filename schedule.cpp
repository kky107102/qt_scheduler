#include "schedule.h"

Schedule::Schedule() {}
Schedule::Schedule(const QString &name, const QDateTime &start, const QDateTime &end,
                   const QString &location, const QString &memo, const QString &period)
    : scheduleName(name), startTime(start), endTime(end), location(location), memo(memo), repeatPeriod(period) {}
Schedule::Schedule(const Schedule& s)
{
    this->scheduleId = s.getScheduleId();
    this->scheduleName = s.getScheduleName();
    this->startTime = s.getStartTime();
    this->endTime = s.getEndTime();
    this->location = s.getLocation();
    this->memo = s.getMemo();
    this->repeatPeriod = s.getPeriod();
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

QString Schedule::getPeriod() const
{
    return repeatPeriod;
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
void Schedule::setPeriod(const QString& period)
{
    this->repeatPeriod = period;
}
