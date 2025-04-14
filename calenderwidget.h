#ifndef CALENDERWIDGET_H
#define CALENDERWIDGET_H

#include <QWidget>
#include <QVector>
#include <QTextCharFormat>
#include "Schedule.h"
#include "dbManager.h"

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

private slots:
    void onClickedDate(); // eidtscheduledialog 호출
};

#endif // CALENDERWIDGET_H
