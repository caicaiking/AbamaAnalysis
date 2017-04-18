#ifndef CLSMAINWINDOW_H
#define CLSMAINWINDOW_H

#include "ui_clsMainWindow.h"

#include "qsinastkinfothread.h"
#include "qeastmoneyblockthread.h"

class clsMainWindow : public QMainWindow, private Ui::clsMainWindow
{
    Q_OBJECT

public:
    explicit clsMainWindow(QWidget *parent = 0);



private:
    QEastMoneyBlockThread *mBlockThread;
};

#endif // CLSMAINWINDOW_H
