#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QFileDialog>
#include <QFile>

#include <cctype>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->wHex, &CustomHexEditor::changedData, this, [this](QByteArray d){
        QString s;
        for(int i = 0; i < d.size(); ++i){
            uchar v = static_cast<uchar>(d[i]);
            if(!std::isprint(v)){
                v = '.';
            }
            s += v;
        }
        ui->pteInput->setPlainText(s);
    });

    connect(ui->pteInput, &CustomPlainTextEdit::keyInput, this, [this](){
        QString val = ui->pteInput->toPlainText();
        ui->wHex->insertData(val.toLocal8Bit());
    });

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    loadSettings();

    mTimer.setInterval(ui->sbTimeout->value());
}

MainWindow::~MainWindow()
{
    saveSettings();

    delete ui;
}


void MainWindow::on_pteInput_textChanged()
{
}

void MainWindow::on_pteHex_textChanged()
{

}

void MainWindow::onTimeout()
{
    QHostAddress addr = QHostAddress(ui->leIP->text());
    ushort port = ui->sbPort->value();
    mSocket.writeDatagram(ui->wHex->data(), addr, port);
    mCounter++;

    ui->statusbar->showMessage(QString::number(mCounter));
}

void MainWindow::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    ui->wHex->insertData(settings.value("hexdata").toByteArray());
    ui->leIP->setText(settings.value("ip", "127.0.0.1").toString());
    ui->sbPort->setValue(settings.value("port", 1234).toUInt());
    ui->sbTimeout->setValue(settings.value("timeout", 100).toUInt());
    ui->checkBox->setChecked(settings.value("repeat").toBool());

    ui->wHex->changedData(ui->wHex->data());
}

void MainWindow::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.setValue("hexdata", ui->wHex->data());
    settings.setValue("ip", ui->leIP->text());
    settings.setValue("port", ui->sbPort->value());
    settings.setValue("timeout", ui->sbTimeout->value());
    settings.setValue("repeat", ui->checkBox->isChecked());
}

void MainWindow::on_sbTimeout_valueChanged(int arg1)
{
    mTimer.setInterval(arg1);
}

void MainWindow::on_pbSendData_clicked()
{
    onTimeout();
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    if(checked){
        mTimer.start();
    }else{
        mTimer.stop();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(nullptr, "Open file...");
    if(!file.isEmpty()){
        QFile f(file);
        f.open(QIODevice::ReadOnly);
        auto b = f.readAll();
        ui->wHex->insertData(b);
    }
}

