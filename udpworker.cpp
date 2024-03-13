#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{
}

/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    textUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);
    textUdpSocket->bind(QHostAddress::LocalHost, BIND_TEXTPORT);
    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
    connect(textUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);

}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();
    QDataStream inStr(&data, QIODevice::ReadOnly);

    if (datagram.senderPort() == BIND_PORT) {
        QDateTime dateTime;
        inStr >> dateTime;

        emit sig_sendTimeToGUI(dateTime);
    }
    else if(datagram.senderPort() == BIND_TEXTPORT)
    {
        QString text(data);
        emit sig_sendTextToGUI(text);
    }

}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

void UDPworker::SendDatagramText(QByteArray data)
{
    textUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_TEXTPORT);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }
    while(textUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = textUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}
