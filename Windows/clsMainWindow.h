#ifndef CLSMAINWINDOW_H
#define CLSMAINWINDOW_H

#include "ui_clsMainWindow.h"

class clsMainWindow : public QMainWindow, private Ui::clsMainWindow
{
    Q_OBJECT

public:
    explicit clsMainWindow(QWidget *parent = 0);

    void setUserName(QString userName);
    void setPriority(int priority);
    void loadCondition();

private slots:
    void on_btnAddUser_clicked();

private:
    QString userName;
    int priority;
};

#endif // CLSMAINWINDOW_H
