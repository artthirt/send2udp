#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pteInput_textChanged();

    void on_pteHex_textChanged();

    void onTimeout();

    void on_sbTimeout_valueChanged(int arg1);

    void on_pbSendData_clicked();

    void on_checkBox_clicked(bool checked);

    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;
    QUdpSocket mSocket;
    QTimer mTimer;
    qint64 mCounter = 0;

    void loadSettings();
    void saveSettings();
};
#endif // MAINWINDOW_H
