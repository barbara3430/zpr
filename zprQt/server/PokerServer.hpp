#pragma once

#include "ClientThread.hpp"

#include <QEvent>
#include <QTcpServer>
#include <QTimer>
#include <QKeyEvent>

class PokerServer : public QTcpServer
{
	Q_OBJECT
protected:
	PokerServer(QObject *parent = 0);
public:
	void stop();
	~PokerServer();
	static PokerServer& getInstance();
public slots:
	/**
	 *	Startuje watek serwera
	 */
	void execute();
private slots:
	/**
	 * W okreslonych odstepach czasu usuwa zakonczone watki klientow
	 */
	void timerAction();
protected:
	/**
	 * wywolywana za kazdym razem, gdy klient probuje nawiazac polaczenie
	 */
	void incomingConnection(int socketId);
	QList<ClientThread*> connections;
	QTimer *timer;
	QThread serverThread;
};
