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
    QDate date;
    QString mod;
    int currentid;

public:
    explicit editScheduleDialog(QString mod, QDate date, Schedule *s = nullptr, QWidget *parent = nullptr);
    ~editScheduleDialog();
    Schedule* getSchedule() const{
        return s;
    }
    QDate getDate() const {
        return this->date;
    }
    QString getMod() const {
        return this->mod;
    }

private:
    Ui::editScheduleDialog *ui;

private slots:
    void onOkClicked();
    void onCancelClicked();
    void saveScheduledata();
    void onChecked(Qt::CheckState state);
signals:
    void signal_done();
};

#endif // EDITSCHEDULEDIALOG_H
