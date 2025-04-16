#include "calenderwidget.h"
#include "ui_calenderwidget.h"
#include <QFontDatabase>

calenderWidget::calenderWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::calenderWidget)
{
    ui->setupUi(this);    
    ui->calendarWidget->showToday();

    QPixmap pixmap(":/resource/calfix_logo.png");
    ui->logoLabel->setPixmap(pixmap);
    ui->logoLabel->setScaledContents(true);

    getSchedules();
    paintSchedules();

    // 캘린더 vertical header 포맷 삭제
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);

    // 검색 버튼 아이콘 설정
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

    // connect 함수
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &calenderWidget::onClickedDate); // 선택된 날짜 일정 조회
    connect(ui->searchBtn, &QPushButton::clicked, this, &calenderWidget::onClickedSearchBtn); // 검색 버튼 클릭
    connect(ui->calendarWidget, &QCalendarWidget::currentPageChanged, this, &calenderWidget::onMonthChanged); // 다른 달 출력
}

calenderWidget::~calenderWidget()
{
    delete ui;
}

bool calenderWidget::getSchedules()
{
    schedules.clear();

    // 현재 선택된 달의 처음 시간, 끝 시간 가져오기
    int year = ui->calendarWidget->yearShown();
    int month = ui->calendarWidget->monthShown();
    QDate firstDate(year, month, 1);
    QTime startOfDay(0, 0, 0);
    QDateTime startDateTime(firstDate, startOfDay);
    QDate lastDate = firstDate.addMonths(1).addDays(-1);
    QTime endOfDay(23, 59, 59);
    QDateTime endDateTime(lastDate, endOfDay);

    // 모든 일정 리스트 조회
    QList<Schedule> sList = dbManager::instance().searchSchedule(startDateTime, endDateTime);

    if (sList.isEmpty())
    {
        return false;
    }

    // 일정 리스트를 schedules 벡터에 추가
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

    // 캘린더 날짜 배경 초기화
    for (QDate d = startPaint; d <= endPaint; d = d.addDays(1)) {
        ui->calendarWidget->setDateTextFormat(d, whiteFormat);
    }

    QTextCharFormat currentMonthFormat = whiteFormat;
    currentMonthFormat.setBackground(QColor("#E4EFE7"));

    for (QDate d = firstDate; d <= lastDate; d = d.addDays(1)) {
        ui->calendarWidget->setDateTextFormat(d, currentMonthFormat);
    }

    // 캘린더에 일정 표시
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(QColor("#B9D2AB"));
    highlightFormat.setForeground(Qt::white);
    highlightFormat.setFontWeight(QFont::Bold);

    for (const Schedule& s : schedules)
    {
        QList<QDate> activeDates = getEffectiveDates(s, startPaint, endPaint); // limit range
        for (const QDate& d : activeDates) {
            if (d > lastDate)
                break;
            if (d >= firstDate)
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
    scheduleDialog->setWindowTitle(date.toString("yyyy년 MM월 dd일"));
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
    searchdialog->setWindowTitle("일정 검색");
    searchdialog->exec();
}

void calenderWidget::onMonthChanged()
{
    getSchedules();
    paintSchedules();
}
