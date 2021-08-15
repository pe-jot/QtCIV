#pragma once

#include <QMainWindow>
#include "satsync.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SatSyncGui; }
QT_END_NAMESPACE

class SatSyncGui : public QMainWindow
{
    Q_OBJECT

public:
    SatSyncGui(QWidget *parent = nullptr);
    ~SatSyncGui();

private slots:
    void on_syncButton_toggled(bool checked);
    void onInitDone(const bool success, const QString message);
    void onActualRxFrequencyChanged();
    void onActualTxFrequencyChanged();
    void onNominalRxFrequencyChanged();
    void onNominalTxFrequencyChanged();

    void on_readNominalButton_clicked();

private:
    Ui::SatSyncGui *ui;
    SatSync *satSync = nullptr;
};
