#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "showScheduleDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showScheduleDialog* a = new showScheduleDialog(QDate::currentDate(),this);
    a->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
