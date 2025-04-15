#ifndef EDITSCHEDULEDIALOG_H
#define EDITSCHEDULEDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QPushButton>
#include "Schedule.h"
#include "dbmanager.h"

namespace Ui {
class editScheduleDialog;
}

class editScheduleDialog : public QDialog
{
    Q_OBJECT
    Schedule* s;

public:
    explicit editScheduleDialog(QString mod,  QDate date, Schedule *s = nullptr, QWidget *parent = nullptr);
    ~editScheduleDialog();
    Schedule* getSchedule();

private:
    Ui::editScheduleDialog *ui;

private slots:
    void onOkClicked();
    void onCancelClicked();
    void saveScheduledata();
signals:
    void signal_done();
};

#endif // EDITSCHEDULEDIALOG_H
