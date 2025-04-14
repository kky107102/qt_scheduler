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
    explicit scheduleListWidget(QWidget *parent = nullptr);
    ~scheduleListWidget();
    QString getTaskName();
public slots:
    void slot_del();
    void slot_show();
signals:
    void delclicked();
    void showclicked();
private:
    Ui::scheduleListWidget *ui;
};

#endif // SCHEDULELISTWIDGET_H
