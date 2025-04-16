#include "searchdialog.h"
#include "ui_searchdialog.h"

searchDialog::searchDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::searchDialog)
{
    ui->setupUi(this);

    ui->searchBtn->setText("");
    ui->searchBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: rgb(153, 188, 133);"
        "    border: 1px solid #ccc;"
        "    border-radius: 5px;"
        "    color: white;"
        "    image: url(:/resource/search.ico);"
        "}"
        "QPushButton:hover {"
        "    background-color: rgb(139, 171, 121);"
        "}"
        );

    connect(ui->searchBtn, &QPushButton::clicked, this, &searchDialog::onClickedSearchBtn);
    connect(ui->chkBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->listWidget, &QListWidget::itemActivated, this, &searchDialog::onClickedListWidget);
}

searchDialog::~searchDialog()
{
    for (Schedule* s : schedules)
        delete s;
    schedules.clear();
    delete ui;
}

// slots
void searchDialog::onClickedSearchBtn()
{
    ui->listWidget->clear();
    schedules.clear();

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
        schedules.push_back(new Schedule(s));
        QString date = s.getStartTime().toString("yyyy-MM-dd hh:mm") + " ~ " + s.getEndTime().toString("yyyy-MM-dd hh:mm");
        QListWidgetItem* item = new QListWidgetItem(date, ui->listWidget);
        item->setData(Qt::UserRole, s.getScheduleId());
    }

    ui->listWidget->show();
}

void searchDialog::onClickedListWidget(QListWidgetItem* item)
{
    int id = item->data(Qt::UserRole).toInt();
    qDebug() << "선택한 일정 ID:" << id;

    for (Schedule* s : schedules)
    {
        if (s->getScheduleId() == id)
        {
            QDate date = s->getStartTime().date();
            editScheduleDialog* infodialog = new editScheduleDialog("info", date, s, this);
            connect(infodialog, &QDialog::finished, infodialog, &QObject::deleteLater);
            infodialog->exec();

            break;
        }
    }
}
