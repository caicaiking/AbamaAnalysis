#include "frmUserInfo.h"
#include "UserfulFunction.h"
#include <QMessageBox>
frmUserInfo::frmUserInfo(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    QStringList l = UsefulFunction::getUserNames();
    this->cmbUserInfoModify->addItems(l);
    this->cmbUserInfoDel->addItems(l);

}
QString frmUserInfo::getStrUserInfo() const
{
    return strUserInfo;
}

void frmUserInfo::setStrUserInfo(const QString &value)
{
    strUserInfo = value;
}


void frmUserInfo::on_buttonBox_accepted()
{
    switch (this->tabWidget->currentIndex()) {
    case 0:
        // qDebug()<< "modify new passwd!";
        if(!UsefulFunction::testUserNameAndPasswd(this->cmbUserInfoModify->currentText(),this->txtOldPassword->text()))
        {
            QMessageBox::critical(this,tr("修改用户信息"),tr("用户密码不正确！"),QMessageBox::Ok);
            break;
        }

        if(txtNewPassword1->text() !=txtNewPassword2->text()
                || txtNewPassword1->text().isEmpty() || txtNewPassword2->text().isEmpty())
        {
            QMessageBox::critical(this,tr("修改用户信息"),tr("用户密码不能为空或两次输入密码不一致！"),QMessageBox::Ok);
            break;
        }

        UsefulFunction::setPassword(this->cmbUserInfoModify->currentText(),this->txtNewPassword1->text());

        QMessageBox::information(this,tr("修改用户信息"),tr("用户密码修改成功！"),QMessageBox::Ok);

        break;

    case 1:

        // qDebug()<< "Add new user";
        btnTest->click();

        if(txtNewUser->text().isEmpty())
        {
            QMessageBox::critical(this,tr("修改用户信息"),
                                  tr("用户名不能为空！"),QMessageBox::Ok);
            break;
        }

        if(txtNUP1->text().isEmpty() || txtNUP2->text().isEmpty())
        {
            QMessageBox::critical(this,tr("修改用户信息"),
                                  tr("用户密码不能为空！"),QMessageBox::Ok);
            break;
        }

        if(txtNUP1->text() != txtNUP2->text())
        {
            QMessageBox::critical(this,tr("修改用户信息"),tr("两次输入密码不一致！"),
                                  QMessageBox::Ok);
            txtNUP1->clear();
            txtNUP2->clear();
            break;
        }

        UsefulFunction::addUser(txtNewUser->text(),txtNUP1->text(),txtPriority->value());

        break;
    case 2:
        qDebug()<< "Delete user";

        int intPriority = UsefulFunction::getPriority(cmbUserInfoDel->currentText());

        if(intPriority==0)
        {
            if(QMessageBox::No == QMessageBox::warning(this,tr("修改用户信息"),
                                                       tr("这是个管理员账户，确定要删除吗？"),QMessageBox::Yes,QMessageBox::No))
                break;
        }

        if(UsefulFunction::delUser(cmbUserInfoDel->currentText()))
        {
            QMessageBox::information(this,tr("修改用户信息"),tr("删除用户成功！"),QMessageBox::Ok);

            QStringList l = UsefulFunction::getUserNames();

            this->cmbUserInfoDel->clear();
            this->cmbUserInfoDel->addItems(l);
        }
        break;

    }

    this->accept();
}

void frmUserInfo::on_buttonBox_rejected()
{
    this->reject();
}

//测试用户名可用性。
void frmUserInfo::on_btnTest_clicked()
{
    if(txtNewUser->text().isEmpty())
    {
        QMessageBox::critical(this,tr("修改用户信息"),tr("用户名不能为空！"),QMessageBox::Ok);
        return;
    }

    if(!UsefulFunction::testUserName(this->txtNewUser->text()) )
    {
        QMessageBox::critical(this,tr("修改用户信息"),tr("用户名已经存在！"),QMessageBox::Ok);
        txtNewUser->clear();
        return;
    }

    txtNewUser->setStyleSheet("background-color: rgb(24, 255, 78);");
}
