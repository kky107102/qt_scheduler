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

    bool getSchedules(); // 데이터베이스에서 일정 목록 가져오는 함수
    void paintSchedules(); // 캘린더에 일정 표시하는 함수

private:
    Ui::calenderWidget *ui;
    QVector<Schedule> schedules;
    showScheduleDialog * scheduleDialog;
    searchDialog * searchdialog;
    QList<QDate> getEffectiveDates(const Schedule& s, const QDate& firstDate, const QDate& lastDate); // 반복되는 일정 처리 함수

private slots:
    void onClickedDate(const QDate &date); // 날짜 클릭 시 eidtscheduledialog 호출하는 슬롯
    void onClickedSearchBtn(); // 검색 버튼 클릭 시 searchdialog 호출하는 슬롯
    void onMonthChanged(); // 현재 달이 바뀌는 이벤트 발생 시 캘린더에 다시 표시하는 함수
};

#endif // CALENDERWIDGET_H
