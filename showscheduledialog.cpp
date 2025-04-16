#include "showscheduledialog.h"
#include "ui_showscheduledialog.h"

showScheduleDialog::showScheduleDialog(const QDate& date, QWidget *parent)
    : date(date), QDialog(parent)
    , ui(new Ui::showScheduleDialog)
{
    ui->setupUi(this);
    ui->dateLbl->setText(getDate().toString("yyyy년 MM월 dd일"));

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

        listItems[i]->setSizeHint(QSize(370,30));
        ui->scheduleList->addItem(listItems[i]);
        ui->scheduleList->setItemWidget(listItems[i], listWidgets[i]);
    }
}

QDate showScheduleDialog::getDate() const{
    return this->date;
}

void showScheduleDialog::newSchedule(){
    dial = new editScheduleDialog("add", this->getDate()); // label로 상단 제목 전달 (추가, 수정, 보기)
    if (dial->exec() == QDialog::Accepted) {
        // editDialog에서 추가한 일정에 현재 선택된 날짜가 포함되어 있다면 리스트에 add
        if (dial->getSchedule()->getStartTime().date() <= this->getDate() && this->getDate() <= dial->getSchedule()->getEndTime().date()){
            schedules.push_back(dial->getSchedule());
            // db에 삽입
            dbManager::instance().insertSchedule(*schedules.back());
            emit add_signal();
        }
        else {
            dbManager::instance().insertSchedule(*(dial->getSchedule()));
        }
    }
}

void showScheduleDialog::addSchedule(){
        auto* item = new QListWidgetItem(ui->scheduleList);
        auto* widget = new scheduleListWidget(schedules.back()->getStartTime(), schedules.back()->getScheduleName(), ui->scheduleList);

        connect(widget, &scheduleListWidget::delclicked, this, &showScheduleDialog::removeSchedule);
        connect(widget, &scheduleListWidget::showclicked, this, &showScheduleDialog::scheduleInfo);

        item->setSizeHint(QSize(370,30));
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
                if (dial->getSchedule()->getStartTime().date() <= this->getDate() && this->getDate() <= dial->getSchedule()->getEndTime().date()){
                    listWidgets[i]->setTaskName(dial->getSchedule()->getScheduleName());
                    listWidgets[i]->setStartTime(dial->getSchedule()->getStartTime());
                    schedules[i] = dial->getSchedule();
                    schedules[i]->setScheduleId(id);
                    // db에서 수정
                    dbManager::instance().modifySchedule(*schedules.at(i));
                }
                else {
                    ui->scheduleList->takeItem(ui->scheduleList->row(listItems[i]));
                    listItems.removeAt(i);
                    listWidgets.removeAt(i);
                    schedules.removeAt(i);
                    dial->getSchedule()->setScheduleId(id);
                    dbManager::instance().modifySchedule(*(dial->getSchedule()));
                }
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
    QList<Schedule> sList = dbManager::instance().getSchedulesForDate(date);

    if (sList.isEmpty())
    {
        qDebug() << "[DEBUG] schedule list is empty";
        return;
    }

    for (const Schedule& s : sList)
    {
        qDebug() << s.getScheduleId();
        Schedule* new_s = new Schedule(s);
        schedules.push_back(new_s);

        QListWidgetItem* item = new QListWidgetItem(ui->scheduleList);
        scheduleListWidget* widget = new scheduleListWidget(new_s->getStartTime(), new_s->getScheduleName(), ui->scheduleList);
        listItems.push_back(item);
        listWidgets.push_back(widget);
    }
}
