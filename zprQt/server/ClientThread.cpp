#include "ClientThread.hpp"

#include <iostream>

ClientThread::ClientThread(int socketId, QObject *parent) :
	QThread(parent), socketDescriptor(socketId)
{
}

void ClientThread::run()
{
	ClientSocket *tcpSocket = new ClientSocket(socketDescriptor, this);
	connect(tcpSocket, SIGNAL(connectionClosed()), SLOT(quit()));
	exec();
}

