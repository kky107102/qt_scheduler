#ifndef EDITSCHEDULEDIALOG_H
#define EDITSCHEDULEDIALOG_H

#include <QDialog>
#include <QDebug>
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
    explicit editScheduleDialog(QString mod, Schedule *s = nullptr, QWidget *parent = nullptr);
    ~editScheduleDialog();
    Schedule* getSchedule();

private:
    Ui::editScheduleDialog *ui;

private slots:
    void onAccepted();
};

#endif // EDITSCHEDULEDIALOG_H
