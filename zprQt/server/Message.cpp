#include "Message.hpp"

#include "Constants.hpp"
#include "MessageQueue.hpp"
#include "Utility.hpp"

unsigned Message::getType()
{
	return msg_type;
}

Message::Message()
{
	id = Rand(); // ZMIENIC
	no_return = false;
	msg_type = MSG_COMMON;
	uint1 = uint2 = uint3 = 0;
}

Message::~Message()
{

}

TableMessage::TableMessage()
{
	id = Rand(); // ZMIENIC
	no_return = false;
	msg_type = MSG_TABLE;
	uint1 = uint2 = uint3 = 0;
}

TableMessage::~TableMessage()
{

}

TableMessage::TableMessage(std::string name, std::string pass, unsigned dest, unsigned op)
{
	id = Rand(); // ZMIENIC
	no_return = false;
	msg_type = MSG_TABLE;
	uint1 = uint2 = uint3 = 0;
	account_name = name;
	account_password = pass;
	dest_table = dest;
	operation = op;

}

void Message::return_msg(bool ok)
{
	if(no_return) {
		return;
	}
	boost::shared_ptr<Message> msg(new Message);
	msg->operation = ok ? RETURN_OK : RETURN_FAIL;
	msg->id = id;

	msg->uint1 = operation; // DEBUGINFO
	msg->str1 = account_name; // DEBUGINFO

	msg->conn = conn;
	MessageQueueOut::getInstance().addMessage(msg);
}
