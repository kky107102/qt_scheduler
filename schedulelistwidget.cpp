#include "schedulelistwidget.h"
#include "ui_schedulelistwidget.h"
#include <QDateTime>

scheduleListWidget::scheduleListWidget(QDateTime startTime, QString task, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::scheduleListWidget)
{
    ui->setupUi(this);
    ui->taskTime->setText(startTime.toString("hh:mm"));
    ui->taskName->setText(task);
    connect(ui->delBtn, &QPushButton::clicked, this, &scheduleListWidget::slot_del);
    connect(ui->showBtn, &QPushButton::clicked, this, &scheduleListWidget::slot_show);
}

scheduleListWidget::~scheduleListWidget()
{
    delete ui;
}

void scheduleListWidget::slot_del() {
    qDebug() << "delclicked";
    emit delclicked(this);
}

void scheduleListWidget::slot_show() {
    emit showclicked(this);
}

QString scheduleListWidget::getTaskName(){
    return ui->taskName->text();
}

void scheduleListWidget::setTaskName(QString name){
    ui->taskName->setText(name);
}
void scheduleListWidget::setStartTime(QDateTime time){
    ui->taskTime->setText(time.toString("hh:mm"));
}
