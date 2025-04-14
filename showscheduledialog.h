#ifndef SHOWSCHEDULEDIALOG_H
#define SHOWSCHEDULEDIALOG_H

#include <QDialog>

namespace Ui {
class showScheduleDialog;
}

class showScheduleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit showScheduleDialog(QWidget *parent = nullptr);
    ~showScheduleDialog();

private:
    Ui::showScheduleDialog *ui;
};

#endif // SHOWSCHEDULEDIALOG_H
