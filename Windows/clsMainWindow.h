#ifndef CLSMAINWINDOW_H
#define CLSMAINWINDOW_H

#include "ui_clsMainWindow.h"
#include "clsStockHisThread.h"

#include "qeastmoneyblockthread.h"

class clsMainWindow : public QMainWindow, private Ui::clsMainWindow
{
    Q_OBJECT

public:
    explicit clsMainWindow(QWidget *parent = 0);


    ~clsMainWindow();

private slots:
    void startGetHisData();
    void on_btnMa_clicked();



    void on_actHsl_triggered();

    void on_btnAttact_clicked();

    void on_btnWeekMa_clicked();

    void on_btnUpdateData_clicked();

private:
    QEastMoneyBlockThread *mBlockThread;
    clsStockHisThread * mStockHisThread;

private:

    int hsl;
    int average;
};

#endif // CLSMAINWINDOW_H
