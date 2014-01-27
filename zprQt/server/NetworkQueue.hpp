#pragma once

#include "Message.hpp"
#include "Utility.hpp"

#include <queue>

#include <QMutex>
#include <QObject>
#include <QWaitCondition>

#include <boost/shared_ptr.hpp>

class NetworkQueue : public QObject
{
	Q_OBJECT
private:
	std::queue<boost::shared_ptr<Message> > message_queue;
	QMutex queue_mutex;
protected:
	NetworkQueue(QObject* = 0);
public:
	~NetworkQueue();
	/**
	 * wzorzec singletona
	 */
	static NetworkQueue& getInstance();
	/**
	 * dodaje wiadomosc do kolejki
	 */
	void addMessage(boost::shared_ptr<Message>);
signals:
	/**
	 * sygnalizuje dodanie nowej wiadomosci do kolejki
	 */
	void messageReady(boost::shared_ptr<Message>);
};
