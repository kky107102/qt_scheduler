#ifndef EDITSCHEDULEDIALOG_H
#define EDITSCHEDULEDIALOG_H

#include <QDialog>

namespace Ui {
class editScheduleDialog;
}

class editScheduleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editScheduleDialog(QWidget *parent = nullptr);
    ~editScheduleDialog();

private:
    Ui::editScheduleDialog *ui;
};

#endif // EDITSCHEDULEDIALOG_H
