#include "NetworkQueue.hpp"

NetworkQueue::NetworkQueue(QObject*)
{
}

NetworkQueue::~NetworkQueue()
{
	while(message_queue.size())
	{
		message_queue.front()->return_msg(false);
		message_queue.pop();
	}
}

NetworkQueue &NetworkQueue::getInstance()
{
	static NetworkQueue instance;
	return instance;
}

void NetworkQueue::addMessage(boost::shared_ptr<Message> msg)
{
	queue_mutex.lock();
	emit messageReady(msg);
	queue_mutex.unlock();
}

