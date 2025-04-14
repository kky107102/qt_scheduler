#include "showscheduledialog.h"
#include "ui_showscheduledialog.h"

showScheduleDialog::showScheduleDialog(QDate date, QWidget *parent)
    : date(date), QDialog(parent)
    , ui(new Ui::showScheduleDialog)
{
    ui->setupUi(this);
    ui->dateLbl->setText(getDate().toString("yyyy년 MM월 dd일"));

    connect(ui->addScheduleBtn, &QPushButton::clicked, this, &showScheduleDialog::newSchedule);
    connect(ui->scheduleList, &QListWidget::clicked, this, &showScheduleDialog::editSchedule);
    //connect(ui->scheduleList, &QListWidget::clicked, this, &showScheduleDialog::addSchedule);

    showScheduleDialog::addSchedule();
    showScheduleDialog::addSchedule();
    for (int idx = 0; idx < listWidgets.size(); idx++){
        currentIdx = idx;
        connect(listWidgets[idx], &scheduleListWidget::showclicked, this, &showScheduleDialog::scheduleInfo);
        connect(listWidgets[idx], &scheduleListWidget::delclicked, this, &showScheduleDialog::removeSchedule);
    }
}

showScheduleDialog::~showScheduleDialog()
{
    delete ui;
}

QDate showScheduleDialog::getDate(){
    return this->date;
}

void showScheduleDialog::newSchedule(){
    add = new editScheduleDialog(); // label로 상단 제목 전달 (추가, 수정, 보기)
    add->show();
}

void showScheduleDialog::editSchedule(){
    edit = new editScheduleDialog(); // label로 상단 제목 전달 (추가, 수정, 보기)
    edit->show();
}

void showScheduleDialog::scheduleInfo(){
    info = new editScheduleDialog(); // label로 상단 제목 전달 (추가, 수정, 보기-currentIdx)
    info->show();
}

void showScheduleDialog::addSchedule(){
    listItems.push_back(new QListWidgetItem(ui->scheduleList));
    listWidgets.push_back(new scheduleListWidget(ui->scheduleList));
    for (int i = 0; i < listWidgets.size(); i++){
        listItems[i]->setSizeHint(listWidgets[i]->sizeHint());
        ui->scheduleList->setItemWidget(listItems[i],listWidgets[i]);
    }
}

void showScheduleDialog::removeSchedule(){
    if (currentIdx != NULL){
        //QListWidgetItem* removeItem =
        ui->scheduleList->takeItem(currentIdx);
        listItems.removeAt(currentIdx);
        listWidgets.removeAt(currentIdx);
    }
}


