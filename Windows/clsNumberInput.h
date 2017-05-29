#ifndef CLSNUMBERINPUT_H
#define CLSNUMBERINPUT_H

#include "ui_clsNumberInput.h"

class clsNumberInput : public QDialog, private Ui::clsNumberInput
{
    Q_OBJECT

public:
    explicit clsNumberInput(QWidget *parent = 0);

    static int getNumber(QWidget *parent, QString windowTitle, QString labelTitle,
                         int defaultValue, int min, int max, int step=1, bool *ok=NULL);
private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
};

#endif // CLSNUMBERINPUT_H
