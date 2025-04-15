#include "searchdialog.h"
#include "ui_searchdialog.h"

searchDialog::searchDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::searchDialog)
{
    ui->setupUi(this);

    connect(ui->searchBtn, &QPushButton::clicked, this, &searchDialog::onClickedSearchBtn);
    connect(ui->chkBtn, &QPushButton::clicked, this, &QDialog::accept);
}

searchDialog::~searchDialog()
{
    delete ui;
}

// slots
void searchDialog::onClickedSearchBtn()
{
    ui->listWidget->clear();

    QString searchNm = ui->scheduleNmLineEdit->text();
    if (searchNm.isEmpty() || searchNm == "일정을 입력하세요.")
    {
        QMessageBox::information(nullptr, "알림", "검색할 일정 이름을 입력하세요.");
        return;
    }
    QList<Schedule> slist = dbManager::instance().searchScheduleName(searchNm);
    if (slist.isEmpty())
    {
        qDebug() << "[DEBUG] no such schedule name";
        return;
    }

    // listWidget에 추가
    for (const Schedule& s : slist)
    {
        QString date = s.getStartTime().toString("yyyy-MM-dd hh:mm") + " ~ " + s.getEndTime().toString("yyyy-MM-dd hh:mm");
        qDebug() << date;
        ui->listWidget->addItem(date);
    }

    ui->listWidget->show();
}
