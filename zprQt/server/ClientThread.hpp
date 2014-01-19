#pragma once

#include "ClientSocket.hpp"

#include <QThread>

class ClientThread : public QThread
{
	Q_OBJECT
public:
	ClientThread(int socketId, QObject *parent = 0);
	/**
	 * uruchamianie watku clienta
	 */
	void run();
signals:
	/**
	 * sygnalizowanie bledu
	 */
	void error(QTcpSocket::SocketError socketerror);
private:
	int socketDescriptor;
};
