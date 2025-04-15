#include "showscheduledialog.h"
#include "ui_showscheduledialog.h"

showScheduleDialog::showScheduleDialog(QDate date, QWidget *parent)
    : date(date), QDialog(parent)
    , ui(new Ui::showScheduleDialog)
{
    ui->setupUi(this);
    ui->dateLbl->setText(getDate().toString("yyyy년 MM월 dd일")); // 현재 date만 받아오고 있어서 datetime받아 date만 세팅하기

    connect(ui->addScheduleBtn, &QPushButton::clicked, this, &showScheduleDialog::newSchedule);
    connect(this, &showScheduleDialog::add_signal, this, &showScheduleDialog::addSchedule);
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

void showScheduleDialog::showSchedule(){
    if (schedules.isEmpty())
    {
        qDebug() << "[DEBUG] schedule is empty";
        return;
    }

    for (int i = 0; i < listItems.size(); i++){
        connect(listWidgets[i], &scheduleListWidget::delclicked, this, &showScheduleDialog::removeSchedule);
        connect(listWidgets[i], &scheduleListWidget::showclicked, this, &showScheduleDialog::scheduleInfo);

        listItems[i]->setSizeHint(listWidgets[i]->sizeHint());
        ui->scheduleList->addItem(listItems[i]);
        ui->scheduleList->setItemWidget(listItems[i], listWidgets[i]);
    }
}

QDate showScheduleDialog::getDate(){
    return this->date;
}

void showScheduleDialog::newSchedule(){
    dial = new editScheduleDialog("add", this->getDate()); // label로 상단 제목 전달 (추가, 수정, 보기)
    if (dial->exec() == QDialog::Accepted) {
        schedules.push_back(dial->getSchedule());

        // db에 삽입
        dbManager::instance().insertSchedule(*schedules.back());

        emit add_signal();
    }
}

void showScheduleDialog::addSchedule(){
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
            dial = new editScheduleDialog("edit", this->getDate(), schedules[i]);
            if (dial->exec() == QDialog::Accepted) {
                int id = schedules[i]->getScheduleId();
                listItems[i] = targetItem;
                listWidgets[i]->setTaskName(dial->getSchedule()->getScheduleName());
                listWidgets[i]->setStartTime(dial->getSchedule()->getStartTime());
                schedules[i] = dial->getSchedule();
                schedules[i]->setScheduleId(id);

                // db에서 수정
                dbManager::instance().modifySchedule(*schedules.at(i));
            }
        }
    }
}

void showScheduleDialog::scheduleInfo(scheduleListWidget* target){
    qDebug() << "info";
    for (int i = 0; i < listWidgets.size(); ++i) {
        if (listWidgets[i] == target){
            dial = new editScheduleDialog("info", this->getDate(), schedules[i]);
            dial->exec();
        }
    }

}

void showScheduleDialog::removeSchedule(scheduleListWidget* target){
    qDebug() << target;
    for (int i = 0; i < listWidgets.size(); ++i) {
        if (listWidgets[i] == target){
            QListWidgetItem* item = listItems[i];
            ui->scheduleList->takeItem(ui->scheduleList->row(item));

            // db에서 삭제
            dbManager::instance().deleteSchedule(*schedules.at(i));

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

    if (sList.isEmpty())
    {
        qDebug() << "[DEBUG] schedule list is empty" << sList.isEmpty();
        return;
    }

    for (const Schedule& s : sList)
    {
        Schedule* new_s = new Schedule(s);
        schedules.push_back(new_s);

        QListWidgetItem* item = new QListWidgetItem(ui->scheduleList);
        scheduleListWidget* widget = new scheduleListWidget(new_s->getStartTime(), new_s->getScheduleName(), ui->scheduleList);
        listItems.push_back(item);
        listWidgets.push_back(widget);
    }
}
