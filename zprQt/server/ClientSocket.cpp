#include "ClientSocket.hpp"

#include "Constants.hpp"
#include "MessageQueue.hpp"
#include "Message.hpp"
#include "NetworkQueue.hpp"
#include "Utility.hpp"

#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>


ClientSocket::ClientSocket(int socketDescriptor, QObject*)
{
	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(readyRead()), this, SLOT(readClient()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(discardClient()));
	connect(&NetworkQueue::getInstance(), SIGNAL(messageReady(boost::shared_ptr<Message>)), this, SLOT(sendResponse(boost::shared_ptr<Message>)));
	socket->setSocketDescriptor(socketDescriptor);
	nextBlockSize = 0;
}

ClientSocket::~ClientSocket()
{
	socket->deleteLater();
}

bool ClientSocket::isActive()
{
	return socket->isOpen();
}

void ClientSocket::readClient()
{
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_3);
	if(nextBlockSize==0)
	{
				 if(socket->bytesAvailable() < sizeof(quint16))
					return;
				 in >> nextBlockSize;
	}
	if(socket->bytesAvailable() < nextBlockSize)
				 return;

	QString name, pass;
	unsigned operation, id, uint1, uint2, uint3, dest_table;
	in >>  name >> pass >> operation >> id >> uint1 >> uint2 >> uint3 >> dest_table;
	if(dest_table != 0)
	{
				 boost::shared_ptr<TableMessage> msg(new TableMessage());
				 msg->dest_table = dest_table;
				 msg->account_name = name.toStdString();
				 msg->account_password = md5(pass.toStdString());
				 msg->id = id;
				 msg->operation = operation;
				 msg->uint1 = uint1;
				 msg->uint2 = uint2;
				 msg->uint3 = uint3;
				 msg->conn.socket = socket->socketDescriptor();
				 MessageQueueIn::getInstance().addMessage(msg);
	}
	else
	{
				 boost::shared_ptr<Message> msg(new Message);
				 msg->account_name = name.toStdString();
				 msg->account_password = md5(pass.toStdString());
				 msg->id = id;
				 msg->operation = operation;
				 msg->uint1 = uint1;
				 msg->uint2 = uint2;
				 msg->uint3 = uint3;
				 msg->conn.socket = socket->socketDescriptor();
				 MessageQueueIn::getInstance().addMessage(msg);
	}
	nextBlockSize = 0;
}

void ClientSocket::sendResponse(boost::shared_ptr<Message> msg)
{
	if(this->socket->socketDescriptor() == msg->conn.socket)
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out << quint16(0) << (unsigned)msg->operation << (unsigned)msg->id << (unsigned)msg->uint1 << (unsigned)msg->uint2
			<< (unsigned)msg->uint3 << QString(msg->str1.c_str());
#if DEBUGLEV >= 4
				 std::cout << (Sync() << (unsigned)msg->operation << " " << (unsigned)msg->id << " " << (unsigned)msg->uint1 << "\n");
#endif
		out.device()->seek(0);
		out << quint16(block.size() - sizeof(quint16));
		socket->write(block);
	}
	else{
		std::cout << "\n";
				 return;
	}
}

void ClientSocket::discardClient()
{
	socket->close();
	emit connectionClosed();
}

