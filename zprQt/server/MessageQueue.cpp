#include "MessageQueue.hpp"

#include "Account.hpp"
#include "AccountBase.hpp"
#include "Constants.hpp"
#include "Lobby.hpp"
#include "Table.hpp"
#include "Utility.hpp"
#include "NetworkQueue.hpp"

#include <boost/thread/locks.hpp>

MessageQueue::MessageQueue()
{
#if DEBUGLEV >= 0
	std::cout << (Sync() << "* MessageQueue running...\n");
#endif
	running = true;
	main_loop = boost::thread(boost::bind(&MessageQueue::prv_mainLoop,this));
}

void MessageQueue::addMessage(boost::shared_ptr<Message> msg)
{
	if(!running) return;
	boost::unique_lock<boost::mutex> lock(queue_mutex);
	message_queue.push(msg);
	queue_empty.notify_one();
}

void MessageQueue::prv_mainLoop()
{
	boost::shared_ptr<Message> message;
	while(running) {
		boost::unique_lock<boost::mutex> lock(queue_mutex);
		while(!message_queue.size()) {
			queue_empty.wait(lock);
			if(!running) return;
		}
		message = message_queue.front();
		message_queue.pop();
		prv_processOperation(message);
	}
}

void MessageQueue::stop()
{
	if(!running) return;

	running = false;

	main_loop.interrupt();
#if DEBUGLEV >= 0
	std::cout << (Sync() << "* MessageQueue stopped...\n");
#endif
}

MessageQueue::~MessageQueue()
{
	stop();
	main_loop.join();
}

void MessageQueueIn::prv_processOperation(boost::shared_ptr<Message> message)
{
	if(!AccountBase::getInstance().verify(message->account_name, message->account_password)) {
#if DEBUGLEV >= 2
		 std::cout << (Sync() << "Message error: login failed\n");
#endif
		message->return_msg(false);
		return;
	}
	Account &account = AccountBase::getInstance().getAccount(message->account_name);
	if(message->operation == ACCOUNT_LOGIN) {
		if(account.isLoggedIn() == true) {
			message->return_msg(false);
			return;
		}
		account.setLoggedIn(true);
		account.setConnection(message->conn);
		message->return_msg(true);
		return;
	}

	if(account.isLoggedIn() == false) {
		message->return_msg(false);
		return;
	}

	if(message->getType() == MSG_TABLE) {
		Lobby::getInstance().sendMessage(boost::static_pointer_cast<TableMessage>(message));
		return;
	}
	switch(message->operation) {
		case ACCOUNT_LOGOUT:
			account.setLoggedIn(false);
			if(account.getTable() != 0) {
				boost::shared_ptr<TableMessage> msg(new TableMessage);
				msg->account_name = message->account_name;
				msg->operation = TABLE_FOLD;
				msg->no_return = true;
				Lobby::getInstance().sendMessage(msg);
			}
			message->return_msg(true);
			return;
		case LIST_TABLES:
			{
				std::list<Table::TableDesc> ret_list = Lobby::getInstance().listTables();
				for(std::list<Table::TableDesc>::iterator it = ret_list.begin(); it != ret_list.end(); ++it) {
					boost::shared_ptr<Message> msg(new Message);
					msg->operation = LIST_TABLES_RETURNMSG;
					msg->uint1 = it->table_id;
					msg->uint2 = it->buy_in;
					msg->uint3 = it->small_blind;
					msg->id = message->id;
					msg->conn = message->conn;
					MessageQueueOut::getInstance().addMessage(msg);
				}
			}
			message->return_msg(true);
			return;
		default:
			message->return_msg(false);
	}
}

void MessageQueueOut::prv_processOperation(boost::shared_ptr<Message> message)
{
#if DEBUGLEV >= 3
	std::cout
		<< (Sync() << "Outgoing message " << decypherOpCode(message->operation) << ": " << message->str1 << ",  " << message->uint1 << ", "
		<< message->uint2 << ", " << message->uint3 << " (id: " << message->id << ")\n");
#endif
	NetworkQueue::getInstance().addMessage(message);
}

MessageQueueIn& MessageQueueIn::getInstance()
{
	static MessageQueueIn instance;
	return instance;
}

MessageQueueOut& MessageQueueOut::getInstance()
{
	static MessageQueueOut instance;
	return instance;
}
