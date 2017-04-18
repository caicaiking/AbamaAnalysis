#ifndef FRMUSERINFO_H
#define FRMUSERINFO_H

#include "ui_frmUserInfo.h"

class frmUserInfo : public QDialog, private Ui::frmUserInfo
{
    Q_OBJECT

public:
    explicit frmUserInfo(QWidget *parent = 0);

    QString getStrUserInfo() const;
    void setStrUserInfo(const QString &value);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_btnTest_clicked();

private:
    QString strUserInfo;
};

#endif // FRMUSERINFO_H
