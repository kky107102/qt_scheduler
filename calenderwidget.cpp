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

    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);

    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &calenderWidget::onClickedDate);
    connect(ui->searchBtn, &QPushButton::clicked, this, &calenderWidget::onClickedSearchBtn);
}

calenderWidget::~calenderWidget()
{
    delete ui;
}

bool calenderWidget::getSchedules()
{
    schedules.clear();
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

    qDebug() << "[DEBUG] schedules: ";
    for (const Schedule& s : sList)
    {
        schedules.append(s);
        qDebug() << schedules.back().getScheduleName();
    }

    return true;
}

void calenderWidget::paintSchedules()
{
    int year = ui->calendarWidget->yearShown();
    int month = ui->calendarWidget->monthShown();
    QDate firstDate(year, month, 1);
    QDate lastDate = firstDate.addMonths(1).addDays(-1);

    QTextCharFormat defaultFormat;
    defaultFormat.setBackground(QColor("#E4EFE7"));
    defaultFormat.setForeground(Qt::black);
    defaultFormat.setFontWeight(QFont::Normal);
    for (QDate d = firstDate; d <= lastDate; d = d.addDays(1)) {
        ui->calendarWidget->setDateTextFormat(d, defaultFormat);
    }

    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(QBrush(QColor("#B9D2AB")));
    highlightFormat.setForeground(Qt::white);
    highlightFormat.setFontWeight(QFont::Bold);
    highlightFormat.setFontUnderline(false);

    for (const Schedule& s : schedules)
    {
        QDate sDate = s.getStartTime().date();
        QDate eDate = s.getEndTime().date();

        QList<QDate> activeDates = getEffectiveDates(s, firstDate, lastDate);

        for (const Schedule& s : schedules)
        {
            QList<QDate> activeDates = getEffectiveDates(s, firstDate, lastDate);
            for (const QDate& d : activeDates) {
                ui->calendarWidget->setDateTextFormat(d, highlightFormat);
            }
        }

        /*
        for (QDate d = sDate; d <= eDate; d = d.addDays(1)) {
            ui->calendarWidget->setDateTextFormat(d, highlightFormat);
        }
        */
    }
}

QList<QDate> calenderWidget::getEffectiveDates(const Schedule& s, const QDate& firstDate, const QDate& lastDate)
{
    QList<QDate> result;
    QDate sDate = s.getStartTime().date();
    QDate eDate = s.getEndTime().date();
    QString type = s.getPeriod();

    if (type == "반복 안 함")
    {
        for (QDate d = sDate; d <= eDate; d = d.addDays(1)) {
            if (d >= firstDate && d <= lastDate)
            {
                result.append(d);
            }
        }
    } else
    {
        QDate d = sDate;
        while (d <= lastDate) {
            if (d >= firstDate)
                result.append(d);

            if (type == "1주 마다")
            {
                d = d.addDays(7);
            }
            else if (type == "1개월 마다")
            {
                d = d.addMonths(1);
            }
        }
    }

    return result;
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

void calenderWidget::onClickedSearchBtn()
{
    qDebug() << "onClickedSearchBtn called";
    searchdialog = new searchDialog(this);
    searchdialog->exec();
}
