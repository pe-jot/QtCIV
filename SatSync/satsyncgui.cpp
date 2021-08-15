#include "satsyncgui.h"
#include "ic705serial.h"
#include "ui_satsyncgui.h"

SatSyncGui::SatSyncGui(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SatSyncGui)
    , satSync(new SatSync)
{
    ui->setupUi(this);

    connect(satSync, &SatSync::initDone, this, &SatSyncGui::onInitDone);
    connect(satSync, &SatSync::rxFrequencyChanged, this, &SatSyncGui::onActualRxFrequencyChanged);
    connect(satSync, &SatSync::txFrequencyChanged, this, &SatSyncGui::onActualTxFrequencyChanged);
    connect(satSync, &SatSync::nominalRxFrequencyChanged, this, &SatSyncGui::onNominalRxFrequencyChanged);
    connect(satSync, &SatSync::nominalTxFrequencyChanged, this, &SatSyncGui::onNominalTxFrequencyChanged);
    satSync->init();
}


SatSyncGui::~SatSyncGui()
{
    disconnect(satSync, &SatSync::rxFrequencyChanged, this, &SatSyncGui::onActualRxFrequencyChanged);
    disconnect(satSync, &SatSync::txFrequencyChanged, this, &SatSyncGui::onActualTxFrequencyChanged);
    disconnect(satSync, &SatSync::nominalRxFrequencyChanged, this, &SatSyncGui::onNominalRxFrequencyChanged);
    disconnect(satSync, &SatSync::nominalTxFrequencyChanged, this, &SatSyncGui::onNominalTxFrequencyChanged);
    delete satSync;
    delete ui;
}


void SatSyncGui::onInitDone(const bool success, const QString message)
{
    disconnect(satSync, &SatSync::initDone, this, &SatSyncGui::onInitDone);

    if (success)
    {
        ui->syncButton->setEnabled(true);
        ui->readNominalButton->setEnabled(true);
        ui->statusbar->showMessage(message, 3000);
    }
    else
    {
        ui->statusbar->showMessage(message);
    }
}


QString HzToString(int Hz)
{
    int MHz = Hz / 1000000;
    int kHz = (Hz - (MHz * 1000000)) / 1000;
    int dHz = (Hz - (MHz * 1000000) - (kHz * 1000)) / 10;

    return QString("%1.%2.%3")
            .arg(MHz, 3, 10, QLatin1Char('0'))
            .arg(kHz, 3, 10, QLatin1Char('0'))
            .arg(dHz, 2, 10, QLatin1Char('0'));
}


void SatSyncGui::onActualRxFrequencyChanged()
{
    ui->actualRxFrequency->display(HzToString(satSync->getRxFrequency()));
}


void SatSyncGui::onActualTxFrequencyChanged()
{
    ui->actualTxFrequency->display(HzToString(satSync->getTxFrequency()));
}


void SatSyncGui::onNominalRxFrequencyChanged()
{
    ui->nominalRxFrequency->setValue((double)(satSync->getNominalRxFrequency() / 1000.0));
}


void SatSyncGui::onNominalTxFrequencyChanged()
{
    ui->nominalTxFrequency->setValue((double)(satSync->getNominalTxFrequency() / 1000.0));
}


void SatSyncGui::on_syncButton_toggled(bool checked)
{
    // Disable frequency change if sync is enabled
    ui->readNominalButton->setEnabled(!checked);
    ui->nominalRxFrequency->setEnabled(!checked);
    ui->nominalTxFrequency->setEnabled(!checked);

    if (checked)
    {
        satSync->setNominalRxFrequency((int)(ui->nominalRxFrequency->value() * 1000));
        satSync->setNominalTxFrequency((int)(ui->nominalTxFrequency->value() * 1000));
        satSync->startSync();
    }
    else
    {
        satSync->stopSync();
    }
}

void SatSyncGui::on_readNominalButton_clicked()
{
    satSync->setNominalRxFrequency(satSync->getRxFrequency());
    satSync->setNominalTxFrequency(satSync->getTxFrequency());
}
