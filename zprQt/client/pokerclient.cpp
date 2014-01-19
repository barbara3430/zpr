#include <QtCore>
#include <QDateTime>
#include <QMessageBox>

#include "pokerclient.h"
#include "constants.h"
#include "tableinfo.h"
#include "model.h"

unsigned PokerClient::messageID = 1;

PokerClient::PokerClient(QObject* parent)
{
	connect(&socket, SIGNAL(connected()), this, SLOT(showConnected()), Qt::DirectConnection);
	connect(&socket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
	connect(&socket, SIGNAL(readyRead()), this, SLOT(receiveData()), Qt::DirectConnection);
	connect(&socket, SIGNAL(error()), this, SLOT(error()));
}

void PokerClient::connectToServer()
{
	host = Model::getInstance().getHostAddress();
	if(socket.state() != QTcpSocket::ConnectedState)
		socket.connectToHost(QHostAddress(host), POKER_PORT);
//	socket.connectToHost(QHostAddress("127.0.0.1"), POKER_PORT);			// TODO test purposes only

	nextBlockSize = 0;
//	if(socket.state() == QTcpSocket::ConnectingState)
//		return true;
//	else
//		return false;
}

void PokerClient::showConnected()
{
	emit connected(true);
}

void PokerClient::receiveData()
{
	QDataStream in(&socket);
	in.setVersion(QDataStream::Qt_4_3);
	forever
	{
		if(nextBlockSize == 0)
		{
			if(socket.bytesAvailable() < sizeof(quint16))
				break;
			in >> nextBlockSize;
		}
		if(socket.bytesAvailable() < nextBlockSize)
			break;

		unsigned operation;
		unsigned id;
		unsigned uint1;
		unsigned uint2;
		unsigned uint3;
		QString str1;

		in >> operation >> id >> uint1 >> uint2 >> uint3 >> str1;
		processResponse(operation, id, uint1, uint2, uint3, str1);
		nextBlockSize = 0;
	}
}

void PokerClient::sendRequest(QString &name, QString &pass, const unsigned &operation, const unsigned &dest_table=0, const unsigned uint1 =0, const unsigned uint2 = 0, const unsigned uint3 = 0)
{
	requestsQueue.push(std::pair<unsigned, unsigned>(operation, messageID));
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_3);
	out << quint16(0) << name << pass << operation << (unsigned)messageID++ << uint1
		<< uint2 << uint3 << dest_table;
	out.device()->seek(0);                          // 'scroll back' to that 0 at the beggining
	out << quint16(block.size() - sizeof(quint16)); // override it with actual data size
	socket.write(block);
}

void PokerClient::processResponse(unsigned &operation, unsigned &id, unsigned &uint1, unsigned &uint2, unsigned &uint3, QString &str1)
{
	unsigned op;
	if(operation != LIST_TABLES_RETURNMSG && operation != LIST_PLAYERS_RETURNMSG && operation != REFRESH_LOBBY && operation != REFRESH_TABLE && operation<106	)
	{
		if(requestsQueue.size() == 0)
		{
			std::cerr << operation << "ktos tutaj robi front() na pustej kolejce\n";
			return;
		}
		op = requestsQueue.front().first;
		if(id == requestsQueue.front().second)
		{
			requestsQueue.pop();
			switch(op)
			{
			case ACCOUNT_LOGIN:
				emit loggedIn( ((operation == RETURN_OK) ? true : false));
				break;
			case LIST_TABLES:
				emit updatedTableList(((operation == RETURN_OK) ? true : false));
				break;
			case LIST_PLAYERS:
				if(whichTable.size()-1 == 0 )
				{
					whichTable.pop();
					emit updatedUsersList( ((operation == RETURN_OK) ? true : false));

				}
				else if(whichTable.size()>=1)
				{
					whichTable.pop();
					int siz = whichTable.size();
					int fronty = whichTable.front();
					requestUsersList(whichTable.front());
				}
				break;
			case TABLE_JOIN:
				emit joinedTable( ((operation == RETURN_OK) ? true : false));
				break;
			case TABLE_LEAVE:
				emit leftTable( ((operation == RETURN_OK) ? true : false));
				break;
			case TABLE_BUYIN:
				emit boughtIn( ((operation == RETURN_OK) ? true : false));
				break;
			case TABLE_PAUSE:
				emit paused( ((operation == RETURN_OK) ? true : false));
				break;
			case TABLE_UNPAUSE:
				emit unpaused( ((operation == RETURN_OK) ? true : false));
				break;
			}
		}
	}
	else
	{
		switch(operation)
		{
		case TABLE_RAISE:
			Model::getInstance().tableList[Model::getInstance().currentTable].last_bet = uint1;
			Model::getInstance().tableList[Model::getInstance().currentTable].last_player = str1;
			Model::getInstance().tableList[Model::getInstance().currentTable].setLastAction(Tableinfo::RAISED);
			//requestUsersList(Model::getInstance().getCurrentTable());
			if(requestsQueue.size() && requestsQueue.front().first == TABLE_RAISE)
			{
				requestsQueue.pop();
				emit raised(str1, uint1);
			}
			else
				emit raised(str1, uint1);
			break;
		case TABLE_CHECK:
			Model::getInstance().tableList[Model::getInstance().currentTable].last_bet = uint1;
			Model::getInstance().tableList[Model::getInstance().currentTable].last_player = str1;
			Model::getInstance().tableList[Model::getInstance().currentTable].setLastAction(Tableinfo::CHECKED);
			//requestUsersList(Model::getInstance().getCurrentTable());
			if(requestsQueue.size() && requestsQueue.front().first == TABLE_CHECK)
			{
				requestsQueue.pop();
				emit checked(str1, uint1);
			}
			else
				emit checked(str1, uint1);
			break;
		case TABLE_FOLD:
			Model::getInstance().tableList[Model::getInstance().currentTable].last_player = str1;
			Model::getInstance().tableList[Model::getInstance().currentTable].setLastAction(Tableinfo::FOLDED);
			//requestUsersList(Model::getInstance().getCurrentTable());
			if(requestsQueue.size() && requestsQueue.front().first == TABLE_FOLD)
			{
				operation == RETURN_OK ? emit folded(str1, uint1) : emit folded(str1, -1);
				requestsQueue.pop();
			}
			else
				emit folded(str1, uint1);
			break;
		case TABLE_ALLIN:
			Model::getInstance().tableList[Model::getInstance().currentTable].last_player = str1;
			Model::getInstance().tableList[Model::getInstance().currentTable].setLastAction(Tableinfo::ALLINED);
			//requestUsersList(Model::getInstance().getCurrentTable());
			if(requestsQueue.size() && requestsQueue.front().first == TABLE_ALLIN)
			{
				operation == RETURN_OK ? emit allIned(str1, uint1) : emit allIned(str1, -1);
				requestsQueue.pop();
			}
			else
				emit allIned(str1, uint1);
			break;
		case LIST_TABLES_RETURNMSG:
			Model::getInstance().addOneTable(Tableinfo(uint1, uint2, uint3));
			break;
		case LIST_PLAYERS_RETURNMSG:
			Model::getInstance().upadateUsersOnTable(whichTable.front(), Playerinfo(str1, uint1, uint2, uint3));
			break;
		case REFRESH_TABLE:
			requestUsersList(Model::getInstance().tableList[Model::getInstance().currentTable].table_id);
			break;
		case REFRESH_LOBBY:
			requestTablesList();
			break;
		case TABLE_REVEAL_PUBLIC_CARD:
			Model::getInstance().tableList[Model::getInstance().currentTable].table_cards.push_back(uint1);
			emit newPublicCard();
			break;
		case TABLE_REVEAL_PRIVATE_CARD:
			Model::getInstance().cards.push_back(uint1);
			emit newPrivateCard();
			break;
		case TABLE_REVEAL_WINNER:
			emit newPrivateCard(str1, uint1);
			break;
		case TABLE_SET_ACTIVE:
			emit activePlayer(str1);
			break;
		case TABLE_NEW_GAME:
			emit newGame();
			break;
		}
	}

}

PokerClient &PokerClient::getInstance()
{
	static PokerClient instance;
	return instance;
}

void PokerClient::connectionClosedByServer()
{
	emit connected(false);
	closeConnection();
}

void PokerClient::error()
{
	closeConnection();
}

void PokerClient::closeConnection()
{
	socket.close();
}

void PokerClient::checkPassword()
{
	name = Model::getInstance().getUserName();
	pass = Model::getInstance().getUserPass();
	sendRequest(name, pass, ACCOUNT_LOGIN);
}

void PokerClient::requestLogout()
{
	sendRequest(name, pass, ACCOUNT_LOGOUT);
}

void PokerClient::requestTablesList()
{
	Model::getInstance().clearTableList();
	sendRequest(name, pass, LIST_TABLES);
}

void PokerClient::requestUsersList(int tableId)
{
	if(whichTable.empty() || whichTable.front() != tableId)
		whichTable.push(tableId);
	Model::getInstance().clearUsersOnTable(tableId);
	sendRequest(name, pass, LIST_PLAYERS, tableId);
}

void PokerClient::requestAllUsers()
{
	std::map<int, Tableinfo>::iterator it = Model::getInstance().tableList.begin();
	int id = it->second.table_id;
	for(;it!=Model::getInstance().tableList.end(); it++)
	{
		whichTable.push(it->second.table_id);
	}
	requestUsersList(id);
}

void PokerClient::requestTableJoin()
{
	Model::getInstance().setCurrentTable(Model::getInstance().getTableToJoin());
	sendRequest(name, pass, TABLE_JOIN, Model::getInstance().getTableToJoin());
}

void PokerClient::requestTableLeave()
{
	sendRequest(name, pass, TABLE_LEAVE, Model::getInstance().tableList[Model::getInstance().currentTable].table_id);
}

void PokerClient::requestTableBuyIn()
{
	sendRequest(name, pass, TABLE_BUYIN, Model::getInstance().tableList[Model::getInstance().currentTable].table_id);
}

void PokerClient::requestTablePause()
{
	sendRequest(name, pass, TABLE_PAUSE, Model::getInstance().tableList[Model::getInstance().currentTable].table_id);
}

void PokerClient::requestTableUnpause()
{
	sendRequest(name, pass, TABLE_UNPAUSE, Model::getInstance().tableList[Model::getInstance().currentTable].table_id);
}

void PokerClient::requestTableRaise()
{
	sendRequest(name, pass, TABLE_RAISE, Model::getInstance().tableList[Model::getInstance().currentTable].table_id, Model::getInstance().getMoneyToRaise());
}

void PokerClient::requestTableCheck()
{
	sendRequest(name, pass, TABLE_CHECK, Model::getInstance().tableList[Model::getInstance().currentTable].table_id);
}

void PokerClient::requestTableFold()
{
	sendRequest(name, pass, TABLE_FOLD, Model::getInstance().tableList[Model::getInstance().currentTable].table_id);
}

void PokerClient::requestTableAllIn()
{
	sendRequest(name, pass, TABLE_ALLIN, Model::getInstance().tableList[Model::getInstance().currentTable].table_id);
}
