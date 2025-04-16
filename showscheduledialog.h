#ifndef SHOWSCHEDULEDIALOG_H
#define SHOWSCHEDULEDIALOG_H

#include <QDialog>
#include <QDate>
#include <QSize>
#include <QListWidgetItem>
#include <QDateTime>
#include "editscheduledialog.h"
#include "scheduleListWidget.h"
#include "schedule.h"

namespace Ui {
class showScheduleDialog;
}

class showScheduleDialog : public QDialog
{
    Q_OBJECT
    QDate date;
    editScheduleDialog* dial;
    QVector<QListWidgetItem*> listItems;
    QVector<scheduleListWidget*> listWidgets;
    QVector<Schedule*> schedules;

public:
    explicit showScheduleDialog(const QDate& date, QWidget *parent = nullptr);
    ~showScheduleDialog();
    QDate getDate() const;

private:
    Ui::showScheduleDialog *ui;

public slots:
    void newSchedule();
    void addSchedule();
    void showSchedule();
    void editSchedule(QListWidgetItem*);
    void scheduleInfo(scheduleListWidget*);
    void removeSchedule(scheduleListWidget*);

    void getScheduleList();
signals:
    void add_signal();
};

#endif // SHOWSCHEDULEDIALOG_H
