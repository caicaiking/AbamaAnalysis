#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include "ui_frmLogin.h"
#include "MainWindow.h"
class frmLogin : public QDialog, private Ui::frmLogin
{
    Q_OBJECT


public:
    explicit frmLogin(QWidget *parent = 0);
private slots:
    void on_btnLogIn_clicked();

private:
    QStringList userName;
    void getUserInfo();

    MainWindow w;
};

#endif // FRMLOGIN_H
