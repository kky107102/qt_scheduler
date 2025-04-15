#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include "schedule.h"
#include "dbmanager.h"

namespace Ui {
class searchDialog;
}

class searchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchDialog(QWidget *parent = nullptr);
    ~searchDialog();

private:
    Ui::searchDialog *ui;

private slots:
    void onClickedSearchBtn();
};

#endif // SEARCHDIALOG_H
