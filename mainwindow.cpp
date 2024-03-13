#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ds = new DialogSender(this);
    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);
    connect(udpWorker, &UDPworker::sig_sendTextToGUI, this, &MainWindow::DisplayText);
    connect(ds, &DialogSender::sig_sendText, this, [&](QString tx)
            {
                QByteArray ba = tx.toUtf8();
        udpWorker->SendDatagramText(ba);

            });
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        //timer->start(TIMER_DELAY);

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    ui->pb_start->setEnabled(false);
    timer->start(TIMER_DELAY);
    ui->pb_stop->setEnabled(true);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));

}

void MainWindow::DisplayText(QString data)
{
    ui->te_result->append(data);
}

void MainWindow::on_pb_stop_clicked()
{
    ui->pb_stop->setEnabled(false);
    timer->stop();
    ui->pb_start->setEnabled(true);
}

void MainWindow::on_pb_textSender_clicked()
{
    ds->show();
}

