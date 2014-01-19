#pragma once

#include "Message.hpp"

#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/utility.hpp>

class MessageQueue : public boost::noncopyable {
	volatile bool running;
	std::queue< boost::shared_ptr<Message> > message_queue;
	boost::mutex queue_mutex;
	boost::condition_variable queue_empty;
	boost::thread main_loop;
	/**
	  * glowna petla dla zadan kolejki
	  */
	void prv_mainLoop();
	/**
	  * zmielenie pojedynczej wiadomosci
	  */
	virtual void prv_processOperation(boost::shared_ptr<Message>) = 0;
protected:
	MessageQueue();
public:
	~MessageQueue();
	/**
	  * dodanie wiadomosci do kolejki
	  */
	void addMessage(boost::shared_ptr<Message>);
	/**
	  * zatrzymanie kolejki
	  */
	void stop();
};

class MessageQueueIn : public MessageQueue {
	void prv_processOperation(boost::shared_ptr<Message>);
public:
	/**
	  * WSZEDZIE SINGLETONY!
	  */
	static MessageQueueIn& getInstance();
};

class MessageQueueOut : public MessageQueue {
	void prv_processOperation(boost::shared_ptr<Message>);
public:
	/**
	  * WSZEDZIE SINGLETONY!
	  */
	static MessageQueueOut& getInstance();
};
