#ifndef CALENDERWIDGET_H
#define CALENDERWIDGET_H

#include <QWidget>

namespace Ui {
class calenderWidget;
}

class calenderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit calenderWidget(QWidget *parent = nullptr);
    ~calenderWidget();

private:
    Ui::calenderWidget *ui;
};

#endif // CALENDERWIDGET_H
