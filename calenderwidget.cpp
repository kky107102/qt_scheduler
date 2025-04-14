#include "calenderwidget.h"
#include "ui_calenderwidget.h"

calenderWidget::calenderWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::calenderWidget)
{
    ui->setupUi(this);
}

calenderWidget::~calenderWidget()
{
    delete ui;
}
