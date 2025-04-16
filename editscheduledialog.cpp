#include "editscheduledialog.h"
#include "ui_editscheduledialog.h"

editScheduleDialog::editScheduleDialog(QString mod, QDate date, Schedule *s, QWidget *parent)
    : date(date), QDialog(parent)
    , ui(new Ui::editScheduleDialog)
{
    ui->setupUi(this);
    // default dateTime
    ui->currentDate->setText(this->getDate().toString("yyyy년 MM월 dd일"));
    ui->startDateTimeEdit->setDateTime(QDateTime(date, QTime().currentTime()));
    ui->endDateTimeEdit->setDateTime(QDateTime(date, QTime().currentTime()));
    ui->repeat_combo->setEnabled(ui->repeat_check->isChecked());

    if (mod == "add"){
        ui->title->setText("일정 추가");
    }
    else if (mod == "edit"){
        ui->title->setText("일정 수정");
        ui->taskNmLineEdit->setText(s->getScheduleName());
        ui->startDateTimeEdit->setDateTime(s->getStartTime());
        ui->endDateTimeEdit->setDateTime(s->getEndTime());
        ui->repeat_combo->setCurrentText(s->getPeriod());
        ui->locationLineEdit->setText(s->getLocation());
        ui->memoTextEdit->setText(s->getMemo());
    }
    else if (mod == "info"){
        ui->title->setText("보기");
        ui->taskNmLineEdit->setText(s->getScheduleName());
        ui->startDateTimeEdit->setDateTime(s->getStartTime());
        ui->endDateTimeEdit->setDateTime(s->getEndTime());
        ui->repeat_check->setEnabled(false);
        ui->repeat_combo->setCurrentText(s->getPeriod());
        ui->locationLineEdit->setText(s->getLocation());
        ui->memoTextEdit->setText(s->getMemo());
        // 수정 제한
        ui->taskNmLineEdit->setReadOnly(true);
        ui->startDateTimeEdit->setReadOnly(true);
        ui->endDateTimeEdit->setReadOnly(true);
        ui->locationLineEdit->setReadOnly(true);
        ui->memoTextEdit->setReadOnly(true);
    }
    connect(ui->repeat_check, &QCheckBox::checkStateChanged, this, &editScheduleDialog::onChecked);
    connect(ui->okBtn, &QPushButton::clicked, this, &editScheduleDialog::onOkClicked);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &editScheduleDialog::onCancelClicked);
}
editScheduleDialog::~editScheduleDialog()
{
    delete ui;
}

void editScheduleDialog::onChecked(Qt::CheckState state){
    if (state == Qt::Unchecked){
        ui->repeat_combo->setEnabled(false);
    }
    else if (state == Qt::Checked){
        ui->repeat_combo->setEnabled(true);
    }
}

void editScheduleDialog::onOkClicked()
{
    if (ui->taskNmLineEdit->text().isEmpty()){
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("경고");
        msgBox.setText("task를 입력하세요.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Retry);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background-color: #FDFAF6;
            }
            QPushButton {
                background-color: #e57373;
                border-radius: 5px;
                min-width: 40px;
                min-height: 20px;
            }
            QPushButton:hover {
                background-color: #ce6767;
            }
        )");
        msgBox.exec();
        return;
    }
    else if(ui->startDateTimeEdit->dateTime() > ui->endDateTimeEdit->dateTime()){
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("경고");
        msgBox.setText("잘못된 시간 설정입니다.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Retry);
        msgBox.setStyleSheet(R"(
            QMessageBox {
                background-color: #FDFAF6;
            }
            QPushButton {
                background-color: #e57373;
                border-radius: 5px;
                min-width: 40px;
                min-height: 20px;
            }
            QPushButton:hover {
                background-color: #ce6767;
            }
        )");
        msgBox.exec();
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
    s->setScheduleName(ui->taskNmLineEdit->text());
    s->setStartTime(ui->startDateTimeEdit->dateTime());
    s->setPeriod(ui->repeat_combo->currentText());
    s->setEndTime(ui->endDateTimeEdit->dateTime());
    s->setLocation(ui->locationLineEdit->text());
    s->setMemo(ui->memoTextEdit->toPlainText());
}

