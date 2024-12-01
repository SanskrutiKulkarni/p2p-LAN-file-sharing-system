#include <QFile>

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "settings.h"
#include "util.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog),
    mCredits(""), mLicense("")
{
    ui->setupUi(this);

    ui->programNameLbl->setText(PROGRAM_NAME);
    ui->programVersionLbl->setText(Util::parseAppVersion(false));
    ui->programDescLbl->setText(PROGRAM_DESC);

    ui->textEdit->setVisible(false);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::onCreditsClicked(bool checked)
{
    if (checked) {
        if (mCredits.isEmpty()) {
            QFile file(":/text/credits.html");
            file.open(QIODevice::ReadOnly);
            mCredits = file.readAll();
        }
        ui->textEdit->setText(mCredits);
    }

    ui->textContent->setVisible(!checked);
    ui->textEdit->setVisible(checked);
    ui->licenseBtn->setChecked(false);
}

void AboutDialog::onLicenseClicked(bool checked)
{
    if (checked) {
        if (mLicense.isEmpty()) {
            QFile file(":/text/gpl-3.0.txt");
            file.open(QIODevice::ReadOnly);
            mLicense = file.readAll();
        }
        ui->textEdit->setText(mLicense);
    }

    ui->textContent->setVisible(!checked);
    ui->textEdit->setVisible(checked);
    ui->creditBtn->setChecked(false);
}
