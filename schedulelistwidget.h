#ifndef SCHEDULELISTWIDGET_H
#define SCHEDULELISTWIDGET_H

#include <QWidget>

namespace Ui {
class scheduleListWidget;
}

class scheduleListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit scheduleListWidget(QDateTime startTime, QString task, QWidget *parent = nullptr);
    ~scheduleListWidget();
    QString getTaskName() const;
    void setTaskName(const QString&);
    void setStartTime(const QDateTime&);
public slots:
    void slot_del();
    void slot_show();
signals:
    void delclicked(scheduleListWidget*);
    void showclicked(scheduleListWidget*);
private:
    Ui::scheduleListWidget *ui;
};

#endif // SCHEDULELISTWIDGET_H
