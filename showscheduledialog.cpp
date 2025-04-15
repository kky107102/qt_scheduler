#include "showscheduledialog.h"
#include "ui_showscheduledialog.h"

showScheduleDialog::showScheduleDialog(QDate date, QWidget *parent)
    : date(date), QDialog(parent)
    , ui(new Ui::showScheduleDialog)
{
    ui->setupUi(this);
    ui->dateLbl->setText(getDate().toString("yyyy년 MM월 dd일")); // 현재 date만 받아오고 있어서 datetime받아 date만 세팅하기

    connect(ui->addScheduleBtn, &QPushButton::clicked, this, &showScheduleDialog::newSchedule);
    connect(this, &showScheduleDialog::show_signal, this, &showScheduleDialog::showSchedule);
    connect(ui->scheduleList, &QListWidget::itemClicked, this, &showScheduleDialog::editSchedule);
    getScheduleList();
    showSchedule();
}

showScheduleDialog::~showScheduleDialog()
{
    // yjseo
    for (Schedule * s : schedules)
        delete s;
    schedules.clear();

    delete ui;
}

QDate showScheduleDialog::getDate(){
    return this->date;
}

void showScheduleDialog::newSchedule(){
    dial = new editScheduleDialog("add"); // label로 상단 제목 전달 (추가, 수정, 보기)
    if (dial->exec() == QDialog::Accepted) {
        schedules.push_back(dial->getSchedule());
        emit show_signal();
    }
}

void showScheduleDialog::showSchedule(){
    auto* item = new QListWidgetItem(ui->scheduleList);
    auto* widget = new scheduleListWidget(schedules.back()->getStartTime(), schedules.back()->getScheduleName(), ui->scheduleList);

    connect(widget, &scheduleListWidget::delclicked, this, &showScheduleDialog::removeSchedule);
    connect(widget, &scheduleListWidget::showclicked, this, &showScheduleDialog::scheduleInfo);

    item->setSizeHint(widget->sizeHint());
    ui->scheduleList->addItem(item);
    ui->scheduleList->setItemWidget(item, widget);

    listItems.push_back(item);
    listWidgets.push_back(widget);
}

void showScheduleDialog::editSchedule(QListWidgetItem* targetItem){
    for (int i = 0; i < listItems.size(); ++i) {
        if (listItems[i] == targetItem){
            dial = new editScheduleDialog("edit", schedules[i]);
            if (dial->exec() == QDialog::Accepted) {
                listItems[i] = targetItem;
                listWidgets[i]->setTaskName(dial->getSchedule()->getScheduleName());
                listWidgets[i]->setStartTime(dial->getSchedule()->getStartTime());
                schedules[i] = dial->getSchedule();
            }
        }
    }
}

void showScheduleDialog::scheduleInfo(scheduleListWidget* target){
    qDebug() << "info";
    for (int i = 0; i < listWidgets.size(); ++i) {
        if (listWidgets[i] == target){
            dial = new editScheduleDialog("info", schedules[i]);
            dial->show();
        }
    }

}

void showScheduleDialog::removeSchedule(scheduleListWidget* target){
    qDebug() << target;
    for (int i = 0; i < listWidgets.size(); ++i) {
        if (listWidgets[i] == target){
            QListWidgetItem* item = listItems[i];
            ui->scheduleList->takeItem(ui->scheduleList->row(item));
            listItems.removeAt(i);
            listWidgets.removeAt(i);
            schedules.removeAt(i);
        }
    }
}

void showScheduleDialog::getScheduleList()
{
    QTime time(0, 0, 0);
    QDateTime dateTime(date, time);
    QList<Schedule> sList = dbManager::instance().getSchedulesForDate(dateTime);

    if (sList.empty())
        return;

    for (const Schedule& s : sList)
    {
        qDebug() << s.getScheduleId();
        Schedule* new_s = new Schedule(s);
        schedules.push_back(new_s);
    }
}
