#include "frmLogin.h"
#include "clsSettings.h"
#include <QCompleter>
#include <QDebug>
#include <QCryptographicHash>
#include "UserfulFunction.h"
#include <qsql.h>
#include <QMessageBox>
frmLogin::frmLogin(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);


    if(!UsefulFunction::createConnection())
    {
        lblStatus->setText(tr("[-]不能连接数据库"));
        return;
    }
    lblStatus->setText(tr("[+]连接数据库成功！"));
    getUserInfo();  //获取已经登录过的用户信息。

    UsefulFunction::createUserTable();
    UsefulFunction::createCategoryTable();
    UsefulFunction::creatCategoryDateilTable();
    UsefulFunction::createMainSheet();
}

void frmLogin::on_btnLogIn_clicked()
{
    bool hasUser=UsefulFunction::testUserName(txtUserName->currentText());
    if(hasUser)
    {
        QMessageBox::information(this,tr("用户登录"),tr("没有此用户！"),QMessageBox::Ok);
        txtUserName->clear();
        txtPassword->clear();
        return;
    }

    bool isPass=UsefulFunction::testUserNameAndPasswd(this->txtUserName->currentText(),this->txtPassword->text());

    if(isPass)
    {
        UsefulFunction::updateLoginTime(this->txtUserName->currentText());
        this->hide();

        w.setUserName(this->txtUserName->currentText());
        w.setPriority(UsefulFunction::getPriority(txtUserName->currentText()));
        w.loadCondition();
        w.showMaximized();
    }
    txtPassword->clear();
}


void frmLogin::getUserInfo()
{
    userName = UsefulFunction::getUserNames();
    QCompleter *completer = new QCompleter(userName, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    this->txtUserName->setCompleter(completer);

    txtUserName->addItems(userName);

}
