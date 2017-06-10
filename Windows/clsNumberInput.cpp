#include "clsNumberInput.h"

clsNumberInput::clsNumberInput(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
}

double clsNumberInput::getNumber(QWidget *parent, QString windowTitle, QString labelTitle,
                              int defaultValue, int min, int max, int step, bool *ok)
{

    clsNumberInput * numberInput = new clsNumberInput(parent);

    numberInput->setWindowTitle(windowTitle);
    numberInput->lblTitle->setText(labelTitle);

    numberInput->spinBox->setMaximum(max);
    numberInput->spinBox->setMinimum(min);
    numberInput->spinBox->setValue(defaultValue);
    numberInput->spinBox->setSingleStep(step);
    numberInput->spinBox->selectAll();

    bool res;
    res= (numberInput->exec()==QDialog::Accepted);

    double spinBoxValue = numberInput->spinBox->value();

    numberInput->close();
    if(ok !=NULL)
        *ok = res;

    return spinBoxValue;


}

void clsNumberInput::on_btnOk_clicked()
{
    this->accept();
}

void clsNumberInput::on_btnCancel_clicked()
{
    this->reject();
}
