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

public:
    explicit editScheduleDialog(QWidget *parent = nullptr);
    ~editScheduleDialog();

private:
    Ui::editScheduleDialog *ui;

private slots:
    void onAccepted();
};

#endif // EDITSCHEDULEDIALOG_H
