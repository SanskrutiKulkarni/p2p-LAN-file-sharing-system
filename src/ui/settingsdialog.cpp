#include <QFileDialog>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "settings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    assign();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onCancelClicked()
{
    reject();
}

void SettingsDialog::onSaveClicked()
{
    Settings* set = Settings::instance();

    QString name = ui->deviceNameLineEdit->text();
    if (!name.isEmpty())
        set->setDeviceName(name);

    set->setBroadcastPort(ui->bcPortSpinBox->value());
    set->setTransferPort(ui->transferPortSpinBox->value());
    set->setFileBufferSize(ui->buffSizeSpinBox->value() * 1024);
    set->setDeviceName(ui->deviceNameLineEdit->text());
    set->setDownloadDir(ui->downDirlineEdit->text());
    set->setBroadcastInterval(ui->bcIntervalSpinBox->value());
    set->setReplaceExistingFile(ui->overwriteCheckBox->isChecked());

    set->saveSettings();

    accept();
}

void SettingsDialog::onResetClicked()
{
    Settings::instance()->reset();
    assign();
}

void SettingsDialog::onSelectDownDirClicked()
{
    QString dirName = Settings::instance()->getDownloadDir();
    QString newDirName = QFileDialog::getExistingDirectory(this, tr("Select a directory"), dirName);

    if (!newDirName.isEmpty())
        ui->downDirlineEdit->setText(newDirName);
}

void SettingsDialog::assign()
{
    Settings* sets = Settings::instance();
    Device me = sets->getMyDevice();

    ui->deviceIdLabel->setText(me.getId());
    ui->ipAddrLabel->setText(me.getAddress().toString());
    ui->osNameLabel->setText(me.getOSName());
    ui->deviceNameLineEdit->setText(me.getName());
    ui->downDirlineEdit->setText(sets->getDownloadDir());

    ui->bcPortSpinBox->setValue(sets->getBroadcastPort());
    ui->transferPortSpinBox->setValue(sets->getTransferPort());
    ui->buffSizeSpinBox->setValue(sets->getFileBufferSize() / 1024);
    ui->bcIntervalSpinBox->setValue(sets->getBroadcastInterval());
    ui->overwriteCheckBox->setChecked(sets->getReplaceExistingFile());
}
