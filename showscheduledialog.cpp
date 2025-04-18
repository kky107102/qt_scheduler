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

    // 일별 일정 리스트에 Item 채우기
    for (int i = 0; i < listItems.size(); i++){
        connect(listWidgets[i], &scheduleListWidget::delclicked, this, &showScheduleDialog::removeSchedule);
        connect(listWidgets[i], &scheduleListWidget::showclicked, this, &showScheduleDialog::scheduleInfo);

        listItems[i]->setSizeHint(QSize(370,40));
        ui->scheduleList->addItem(listItems[i]);
        ui->scheduleList->setItemWidget(listItems[i], listWidgets[i]);
    }
}

QDate showScheduleDialog::getDate() const{
    return this->date;
}

void showScheduleDialog::newSchedule(){
    dial = new editScheduleDialog("add", this->getDate()); // label로 상단 제목 전달 (추가, 수정, 보기)
    dial->setWindowTitle("일정 추가");
    if (dial->exec() == QDialog::Accepted) {
        // editDialog에서 추가한 일정에 현재 선택된 날짜가 포함되어 있다면
        if (dial->getSchedule()->getStartTime().date() <= this->getDate() && this->getDate() <= dial->getSchedule()->getEndTime().date()){
            // 일별 일정 리스트와 db에 추가
            schedules.push_back(dial->getSchedule());
            dbManager::instance().insertSchedule(*schedules.back());
            emit add_signal();
        }
        // editDialog에서 추가한 일정에 현재 선택된 날짜가 포함되어 있지 않다면
        else {
            // db에만 추가
            dbManager::instance().insertSchedule(*(dial->getSchedule()));
        }
    }
}

void showScheduleDialog::addSchedule(){
    // 새로 추가된 일정을 리스트에 추가
    auto* item = new QListWidgetItem(ui->scheduleList);
    auto* widget = new scheduleListWidget(schedules.back()->getStartTime(), schedules.back()->getScheduleName(), ui->scheduleList);

    connect(widget, &scheduleListWidget::delclicked, this, &showScheduleDialog::removeSchedule);
    connect(widget, &scheduleListWidget::showclicked, this, &showScheduleDialog::scheduleInfo);

    item->setSizeHint(QSize(370,40));
    ui->scheduleList->addItem(item);
    ui->scheduleList->setItemWidget(item, widget);

    listItems.push_back(item);
    listWidgets.push_back(widget);
}

void showScheduleDialog::editSchedule(QListWidgetItem* targetItem){
    for (int i = 0; i < listItems.size(); ++i) {
        if (listItems[i] == targetItem){
            dial = new editScheduleDialog("edit", this->getDate(), schedules[i], this);
            dial->setWindowTitle("일정 수정");
            if (dial->exec() == QDialog::Accepted) {
                int id = schedules[i]->getScheduleId();
                // editDialog에서 수정한 일정에 현재 선택된 날짜가 포함되어 있다면
                if (dial->getSchedule()->getStartTime().date() <= this->getDate() && this->getDate() <= dial->getSchedule()->getEndTime().date()){
                    // 리스트 및 db 수정
                    listWidgets[i]->setTaskName(dial->getSchedule()->getScheduleName());
                    listWidgets[i]->setStartTime(dial->getSchedule()->getStartTime());
                    schedules[i] = dial->getSchedule();
                    schedules[i]->setScheduleId(id);
                    dbManager::instance().modifySchedule(*schedules.at(i));
                }
                // editDialog에서 수정한 일정에 현재 선택된 날짜가 포함되어 있지 않다면
                else {
                    // 현재 날짜에서는 제거
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
            dial->setWindowTitle("일정 보기");
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
