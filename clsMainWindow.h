#ifndef CLSMAINWINDOW_H
#define CLSMAINWINDOW_H

#include "ui_clsMainWindow.h"

class clsMainWindow : public QMainWindow, private Ui::clsMainWindow
{
    Q_OBJECT

public:
    explicit clsMainWindow(QWidget *parent = 0);
};

#endif // CLSMAINWINDOW_H
