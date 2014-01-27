#include "PokerServer.hpp"

#include "Constants.hpp"

#include <iostream>

#include <QEvent>
#include <QKeyEvent>
#include <QThread>

PokerServer::PokerServer(QObject *parent) : QTcpServer(parent)
{
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerAction()));
	timer->start(10000);
	qRegisterMetaType<boost::shared_ptr<Message> >("boost::shared_ptr<Message>");

	this->moveToThread(&serverThread);

	this->connect(&serverThread, SIGNAL(started()), SLOT(execute()));

	serverThread.start();

}

void PokerServer::stop()
{
	serverThread.quit();
	qDeleteAll(connections);
	connections.clear();
}


PokerServer::~PokerServer()
{
	stop();
}

void PokerServer::execute()
{
	if(!listen(QHostAddress::Any, POKER_PORT))
	{
		std::cerr << "Cannot bind port";
		return;
	}
}

void PokerServer::timerAction()
{
	QMutableListIterator<ClientThread*> i(connections);
	while (i.hasNext()) {
		ClientThread* t = i.next();

		if (t->isFinished()) {
			delete t;
			i.remove();
		}
	}
}

void PokerServer::incomingConnection(int socketId)
{
	ClientThread *thread = new ClientThread(socketId, this);
	connections.append(thread);
	thread->start();
}


PokerServer& PokerServer::getInstance()
{
	static PokerServer instance;
	return instance;
}
