#include "schedulelistwidget.h"
#include "ui_schedulelistwidget.h"

scheduleListWidget::scheduleListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::scheduleListWidget)
{
    ui->setupUi(this);
    connect(ui->delBtn, &QPushButton::clicked, this, &scheduleListWidget::slot_del);
    connect(ui->showBtn, &QPushButton::clicked, this, &scheduleListWidget::slot_show);
}

scheduleListWidget::~scheduleListWidget()
{
    delete ui;
}

void scheduleListWidget::slot_del() {
    emit delclicked();
}

void scheduleListWidget::slot_show() {
    emit showclicked();
}

QString scheduleListWidget::getTaskName(){
    return ui->taskName->text();
}
