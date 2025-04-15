#include "calenderwidget.h"
#include "ui_calenderwidget.h"

calenderWidget::calenderWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::calenderWidget)
{
    ui->setupUi(this);
    ui->calendarWidget->showToday();
    getSchedules();
    paintSchedules();

    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &calenderWidget::onClickedDate);
}

calenderWidget::~calenderWidget()
{
    delete ui;
}

bool calenderWidget::getSchedules()
{
    int year = ui->calendarWidget->yearShown();
    int month = ui->calendarWidget->monthShown();
    QDate firstDate(year, month, 1);
    QTime startOfDay(0, 0, 0);
    QDateTime startDateTime(firstDate, startOfDay);

    QDate lastDate = firstDate.addMonths(1).addDays(-1);
    QTime endOfDay(23, 59, 59);
    QDateTime endDateTime(lastDate, endOfDay);

    qDebug() << "startDate: " << startDateTime.toString(Qt::ISODate);
    qDebug() << "endDate:" << endDateTime.toString(Qt::ISODate);

    QList<Schedule> sList = dbManager::instance().searchSchedule(startDateTime, endDateTime);
    for (const Schedule& s : sList)
    {
        schedules.append(s);
        qDebug() << s.getScheduleId() << " "
                << " " << s.getStartTime() << " " << s.getEndTime();
    }

    return true;
}

void calenderWidget::paintSchedules()
{
    qDebug() << "paintSchedules() called";

    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::blue);

    for (const Schedule& s : schedules)
    {
        QDate sDate = s.getStartTime().date();
        QDate eDate = s.getEndTime().date();

        for (QDate d = sDate; d <= eDate; d = d.addDays(1))
            ui->calendarWidget->setDateTextFormat(d, highlightFormat);
    }

}

// slots
void calenderWidget::onClickedDate(const QDate &date)
{
    qDebug() << "onClickedDate called";
    scheduleDialog = new showScheduleDialog(date, this);
    scheduleDialog->exec();
}
