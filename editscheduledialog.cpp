#include "editscheduledialog.h"
#include "ui_editscheduledialog.h"

editScheduleDialog::editScheduleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editScheduleDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &editScheduleDialog::onAccepted);
}

editScheduleDialog::~editScheduleDialog()
{
    delete ui;
}

void editScheduleDialog::onAccepted()
{
    QString _name = ui->taskNmLineEdit->text();
    QDateTime _startTm = ui->startDateTimeEdit->dateTime();
    QDateTime _endTm = ui->endDateTimeEdit->dateTime();
    QString _loc = ui->locationLineEdit->text();
    QString _memo = ui->memoTextEdit->toPlainText();

    qDebug() << "[DEBUG] onAccepted() called";

    Schedule s(_name, _startTm, _endTm, _loc, _memo);

    // db에 추가
    int id = dbManager::instance().insertSchedule(s);
    if (id == -1) {
        qDebug() << "[ERROR] fail ";
        return;
    }
}
