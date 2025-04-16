#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QVector>
#include <QListWidgetItem>
#include "editscheduledialog.h"
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
    QVector<Schedule *> schedules;

public slots:
    void onClickedSearchBtn(); // 검색 버튼 처리 함수
    void onClickedListWidget(QListWidgetItem* item); // 검색된 일정 리스트 클릭 처리 함수
};

#endif // SEARCHDIALOG_H
