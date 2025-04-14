#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /////
    editScheduleDialog editDialog(this);
    editDialog.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
