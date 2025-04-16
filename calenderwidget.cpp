#include "calenderwidget.h"
#include "ui_calenderwidget.h"
#include <QFontDatabase>

calenderWidget::calenderWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::calenderWidget)
{
    ui->setupUi(this);    
    ui->calendarWidget->showToday();
    QPixmap pixmap(":/resource/calfix_logo.png"); // 리소스 또는 경로 사용
    ui->logoLabel->setPixmap(pixmap);
    ui->logoLabel->setScaledContents(true);
    getSchedules();
    paintSchedules();

    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->searchBtn->setText("");
    ui->searchBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: rgb(153, 188, 133);"
        "    border: 1px solid #ccc;"
        "    border-radius: 5px;"
        "    color: white;"
        "    image: url(:/resource/search.ico);"
        "}"
        "QPushButton:hover {"
        "    background-color: rgb(139, 171, 121);"
        "}"
        );

    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &calenderWidget::onClickedDate);
    connect(ui->searchBtn, &QPushButton::clicked, this, &calenderWidget::onClickedSearchBtn);
    connect(ui->calendarWidget, &QCalendarWidget::currentPageChanged, this, &calenderWidget::onMonthChanged);
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

    QTextCharFormat whiteFormat;
    whiteFormat.setBackground(Qt::white);
    whiteFormat.setForeground(Qt::black);
    whiteFormat.setFontWeight(QFont::Normal);

    QDate startPaint = firstDate.addDays(-14);
    QDate endPaint = lastDate.addDays(14);

    for (QDate d = startPaint; d <= endPaint; d = d.addDays(1)) {
        ui->calendarWidget->setDateTextFormat(d, whiteFormat);
    }

    QTextCharFormat currentMonthFormat = whiteFormat;
    currentMonthFormat.setBackground(QColor("#E4EFE7"));

    for (QDate d = firstDate; d <= lastDate; d = d.addDays(1)) {
        ui->calendarWidget->setDateTextFormat(d, currentMonthFormat);
    }

    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(QColor("#B9D2AB"));
    highlightFormat.setForeground(Qt::white);
    highlightFormat.setFontWeight(QFont::Bold);

    for (const Schedule& s : schedules)
    {
        QList<QDate> activeDates = getEffectiveDates(s, startPaint, endPaint); // limit range
        for (const QDate& d : activeDates) {
            ui->calendarWidget->setDateTextFormat(d, highlightFormat);
        }
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
            if (d >= firstDate && d <= lastDate) {
                result.append(d);
            }
        }
    }
    else
    {
        QDate periodStart = sDate;
        int duration = sDate.daysTo(eDate); // 일정의 길이 계산 (몇 일짜리 일정인지)

        while (periodStart <= lastDate)
        {
            QDate periodEnd = periodStart.addDays(duration);

            for (QDate d = periodStart; d <= periodEnd; d = d.addDays(1)) {
                if (d >= firstDate && d <= lastDate) {
                    result.append(d);
                }
            }

            // 다음 반복 주기로 이동
            if (type == "1주 마다")
                periodStart = periodStart.addDays(7);
            else if (type == "1개월 마다")
                periodStart = periodStart.addMonths(1);
        }
    }

    return result;
}

// slots
void calenderWidget::onClickedDate(const QDate &date)
{
    qDebug() << "onClickedDate called";
    scheduleDialog = new showScheduleDialog(date);
    if (scheduleDialog->exec() == QDialog::Rejected)
    {
        getSchedules();
        paintSchedules();
    }
}

void calenderWidget::onClickedSearchBtn()
{
    qDebug() << "onClickedSearchBtn called";
    searchdialog = new searchDialog();
    searchdialog->exec();
}

void calenderWidget::onMonthChanged()
{
    getSchedules();
    paintSchedules();
}
