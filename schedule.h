#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QString>
#include <QDateTime>

class Schedule
{
private:
    int scheduleId;
    QString scheduleName;
    QDateTime startTime;
    QDateTime endTime;
    QString location;
    QString memo;
    QString repeatPeriod;

public:
    Schedule();
    Schedule(const QString &name, const QDateTime &start, const QDateTime &end,
             const QString &location, const QString &memo, const QString &period);
    Schedule(const Schedule& s);
    ~Schedule();

    // getter
    int getScheduleId() const;
    QString getScheduleName() const;
    QDateTime getStartTime() const;
    QDateTime getEndTime() const;
    QString getLocation() const;
    QString getMemo() const;
    QString getPeriod() const;

    // setter
    void setScheduleId(const int&);
    void setScheduleName(const QString& name);
    void setStartTime(const QDateTime& start);
    void setEndTime(const QDateTime& end);
    void setLocation(const QString& location);
    void setMemo(const QString& memo);
    void setPeriod(const QString& period);
};

#endif // SCHEDULE_H
