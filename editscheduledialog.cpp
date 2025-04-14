#include "editscheduledialog.h"
#include "ui_editscheduledialog.h"

editScheduleDialog::editScheduleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editScheduleDialog)
{
    ui->setupUi(this);
}

editScheduleDialog::~editScheduleDialog()
{
    delete ui;
}
