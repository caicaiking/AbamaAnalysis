#include "clsMainWindow.h"
#include "frmUserInfo.h"
clsMainWindow::clsMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);


}

void clsMainWindow::setUserName(QString userName)
{
    this->lblUserName->setText(userName);
}

void clsMainWindow::setPriority(int priority)
{
    this->priority = priority;
    btnAddUser->setVisible((this->priority == 0));
}

void clsMainWindow::loadCondition()
{

}

void clsMainWindow::on_btnAddUser_clicked()
{
    frmUserInfo *dlg = new frmUserInfo(this);
    dlg->setWindowTitle(tr("用户信息"));

    dlg->exec();
}
