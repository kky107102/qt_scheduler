#include "editscheduledialog.h"
#include "ui_editscheduledialog.h"

editScheduleDialog::editScheduleDialog(QString mod, Schedule *s, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editScheduleDialog)
{
    ui->setupUi(this);
    // default dateTime
    // ui->startDateTimeEdit->setDateTime(dateTime);
    // ui->endDateTimeEdit->setDateTime(dateTime);
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
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &editScheduleDialog::onAccepted);
}
editScheduleDialog::~editScheduleDialog()
{
    delete ui;
}

void editScheduleDialog::onAccepted()
{
    // QString _name = ui->taskNmLineEdit->text();
    // QDateTime _startTm = ui->startDateTimeEdit->dateTime();
    // QDateTime _endTm = ui->endDateTimeEdit->dateTime();
    // QString _loc = ui->locationLineEdit->text();
    // QString _memo = ui->memoTextEdit->toPlainText();

    // qDebug() << "[DEBUG] onAccepted() called";

    // Schedule s(_name, _startTm, _endTm, _loc, _memo);

    // // db에 추가
    // int id = dbManager::instance().insertSchedule(s);
    // if (id == -1) {
    //     qDebug() << "[ERROR] fail ";
    //     return;
    // }
    s = new Schedule();
    s->setScheduleName(ui->taskNmLineEdit->text()); // 예외 처리: 비어있지 않게
    s->setStartTime(ui->startDateTimeEdit->dateTime());
    s->setEndTime(ui->endDateTimeEdit->dateTime()); // 예외 처리: 시간이 start보다 이르지 않게
    s->setLocation(ui->locationLineEdit->text());
    s->setMemo(ui->memoTextEdit->toPlainText());
    qDebug() << "accepted";
}

Schedule* editScheduleDialog::getSchedule(){
    return s;
}
