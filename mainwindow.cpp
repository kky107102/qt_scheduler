#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "calenderwidget.h"
#include "showScheduleDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //showScheduleDialog* a = new showScheduleDialog(QDate::currentDate(),this);
    calenderWidget* calWidget = new calenderWidget(this);
    setCentralWidget(calWidget);

    this->setStyleSheet("background-color: #FDFAF6;");
    //a->show();
    calWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
