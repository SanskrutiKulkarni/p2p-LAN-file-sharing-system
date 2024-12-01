#include <QMessageBox>

#include "receiverselectordialog.h"
#include "ui_receiverselectordialog.h"

#include "model/devicelistmodel.h"
#include "model/device.h"

ReceiverSelectorDialog::ReceiverSelectorDialog(DeviceListModel* model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReceiverSelectorDialog),
    mModel(model)
{
    ui->setupUi(this);

    ui->listView->setModel(mModel);
    ui->listView->setCurrentIndex(QModelIndex());

    model->refresh();
}

ReceiverSelectorDialog::~ReceiverSelectorDialog()
{
    delete ui;
}

Device ReceiverSelectorDialog::getSelectedDevice() const
{
    QModelIndex currIndex = ui->listView->currentIndex();
    if (currIndex.isValid()) {
        return mModel->device(currIndex.row());
    }

    return Device();
}

QVector<Device> ReceiverSelectorDialog::getSelectedDevices() const
{
    QVector<Device> devices;
    QItemSelectionModel* selModel = ui->listView->selectionModel();
    if (selModel) {

        QModelIndexList selected = selModel->selectedIndexes();
        for (auto selectedIndex : selected) {
            if (selectedIndex.isValid()) {
                devices.push_back(mModel->device(selectedIndex.row()));
            }
        }
    }

    return devices;
}

void ReceiverSelectorDialog::onSendClicked()
{
    QModelIndex currIndex = ui->listView->currentIndex();
    if (currIndex.isValid())
        accept();
    else
        QMessageBox::information(this, tr("Info"), tr("Please select receivers."));
}

void ReceiverSelectorDialog::onRefreshClicked()
{
    mModel->refresh();
}
