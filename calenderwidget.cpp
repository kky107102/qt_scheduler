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

    QList<Schedule> sList = dbManager::instance().searchSchedule(startDateTime, endDateTime);

    if (sList.isEmpty())
    {
        return false;
    }

    for (const Schedule& s : sList)
        schedules.append(s);

    return true;
}

void calenderWidget::paintSchedules()
{
    int year = ui->calendarWidget->yearShown();
    int month = ui->calendarWidget->monthShown();
    QDate firstDate(year, month, 1);
    QDate lastDate = firstDate.addMonths(1).addDays(-1);

    QTextCharFormat defaultFormat;
    defaultFormat.setBackground(Qt::white);
    for (QDate d = firstDate; d <= lastDate; d = d.addDays(1))
        ui->calendarWidget->setDateTextFormat(d, defaultFormat);


    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::green);

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
    if (scheduleDialog->exec() == QDialog::Rejected)
    {
        getSchedules();
        paintSchedules();
    }
}
