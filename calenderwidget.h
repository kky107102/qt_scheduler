#ifndef CALENDERWIDGET_H
#define CALENDERWIDGET_H

#include <QWidget>
#include <QVector>
#include <QTextCharFormat>
#include "showScheduleDialog.h"
#include "searchdialog.h"
#include "Schedule.h"

namespace Ui {
class calenderWidget;
}

class calenderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit calenderWidget(QWidget *parent = nullptr);
    ~calenderWidget();

    bool getSchedules();
    void paintSchedules();

private:
    Ui::calenderWidget *ui;
    QVector<Schedule> schedules;
    showScheduleDialog * scheduleDialog;
    searchDialog * searchdialog;
    QList<QDate> getEffectiveDates(const Schedule& s, const QDate& firstDate, const QDate& lastDate);

private slots:
    void onClickedDate(const QDate &date); // eidtscheduledialog 호출
    void onClickedSearchBtn();
};

#endif // CALENDERWIDGET_H
