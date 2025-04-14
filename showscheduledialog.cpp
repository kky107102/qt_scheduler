#include "showscheduledialog.h"
#include "ui_showscheduledialog.h"

showScheduleDialog::showScheduleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::showScheduleDialog)
{
    ui->setupUi(this);
}

showScheduleDialog::~showScheduleDialog()
{
    delete ui;
}
