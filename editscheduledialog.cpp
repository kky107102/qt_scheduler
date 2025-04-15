#include "editscheduledialog.h"
#include "ui_editscheduledialog.h"

editScheduleDialog::editScheduleDialog(QString mod, QDate date, Schedule *s, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editScheduleDialog)
{
    ui->setupUi(this);
    // default dateTime
    ui->startDateTimeEdit->setDateTime(QDateTime(date, QTime().currentTime()));
    ui->endDateTimeEdit->setDateTime(QDateTime(date, QTime().currentTime()));
    if (mod == "add"){
        ui->title->setText("일정 추가");
    }
    else if (mod == "edit"){
            qDebug() << s->getScheduleName();
        ui->title->setText("일정 수정");
        ui->taskNmLineEdit->setText(s->getScheduleName());
        ui->startDateTimeEdit->setDateTime(s->getStartTime());
        ui->endDateTimeEdit->setDateTime(s->getEndTime());
        ui->locationLineEdit->setText(s->getLocation());
        ui->memoTextEdit->setText(s->getMemo());
    }
    else if (mod == "info"){
        ui->title->setText("보기");
        ui->taskNmLineEdit->setText(s->getScheduleName());
        ui->startDateTimeEdit->setDateTime(s->getStartTime());
        ui->endDateTimeEdit->setDateTime(s->getEndTime());
        ui->locationLineEdit->setText(s->getLocation());
        ui->memoTextEdit->setText(s->getMemo());
        // 수정 제한
        ui->taskNmLineEdit->setReadOnly(true);
        ui->startDateTimeEdit->setReadOnly(true);
        ui->endDateTimeEdit->setReadOnly(true);
        ui->locationLineEdit->setReadOnly(true);
        ui->memoTextEdit->setReadOnly(true);
    } 
    connect(ui->okBtn, &QPushButton::clicked, this, &editScheduleDialog::onOkClicked);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &editScheduleDialog::onCancelClicked);
}
editScheduleDialog::~editScheduleDialog()
{
    delete ui;
}

void editScheduleDialog::onOkClicked()
{
    if (ui->taskNmLineEdit->text().isEmpty()){
        QMessageBox::critical(this,"Critical 다이얼로그","task를 입력하세요.",QMessageBox::Retry);
        return;
    }
    else if(ui->startDateTimeEdit->dateTime() >= ui->endDateTimeEdit->dateTime()){
        QMessageBox::critical(this,"Critical 다이얼로그","잘못된 시간 설정입니다.",QMessageBox::Retry);
        //qDebug() << "잘못된 시간설정입니다.";
        return;
    }
    else{
        saveScheduledata();
        accept();
        qDebug() << "accepted";
    }
}

void editScheduleDialog::onCancelClicked(){
    reject();
}

void editScheduleDialog::saveScheduledata()
{
    s = new Schedule();
    s->setScheduleName(ui->taskNmLineEdit->text()); // 예외 처리: 비어있지 않게
    s->setStartTime(ui->startDateTimeEdit->dateTime());
    s->setEndTime(ui->endDateTimeEdit->dateTime()); // 예외 처리: 시간이 start보다 이르지 않게
    s->setLocation(ui->locationLineEdit->text());
    s->setMemo(ui->memoTextEdit->toPlainText());
}

Schedule* editScheduleDialog::getSchedule(){
    return s;
}
