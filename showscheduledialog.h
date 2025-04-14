#ifndef SHOWSCHEDULEDIALOG_H
#define SHOWSCHEDULEDIALOG_H

#include <QDialog>
#include <QDate>
#include <QListWidgetItem>
#include "editscheduledialog.h"
#include "scheduleListWidget.h"


namespace Ui {
class showScheduleDialog;
}

class showScheduleDialog : public QDialog
{
    Q_OBJECT
    QDate date;
    int currentIdx;
    editScheduleDialog* add;
    editScheduleDialog* edit;
    editScheduleDialog* info;
    QVector<QListWidgetItem*> listItems;
    QVector<scheduleListWidget*> listWidgets;

public:
    explicit showScheduleDialog(QDate date, QWidget *parent);
    ~showScheduleDialog();
    QDate getDate();

private:
    Ui::showScheduleDialog *ui;

public slots:
    void newSchedule();
    void addSchedule();
    void scheduleInfo();
    void editSchedule();
    void removeSchedule();
};

#endif // SHOWSCHEDULEDIALOG_H
