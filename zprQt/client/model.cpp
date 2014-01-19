#include "model.h"
#include <iostream>

Model::Model()
{
}

Model &Model::getInstance()
{
	static Model instance;
	return instance;
}

void Model::setUserName(const QString &name)
{
	this->userName = name;
}

QString Model::getUserName() const
{
	return userName;
}

void Model::setUserPass(const QString &pass)
{
	this->userPass = pass;
}

QString Model::getUserPass() const
{
	return userPass;
}

void Model::clearTableList()
{
	tableList.clear();
}

void Model::addOneTable(Tableinfo table)
{
	tableList[table.table_id] = table;
}

void Model::clearUsersOnTable(int tableNumber)
{
	tableList[tableNumber].players.clear();
	tableList[tableNumber].playersQueue.clear();
}

void Model::upadateUsersOnTable(int tableNumber, Playerinfo player)
{
	tableList[tableNumber].players[player.name] = player;
	tableList[tableNumber].playersQueue.push_back(player.name);
}

void Model::setCurrentTable(const int& tableNumber)
{
	currentTable = tableNumber;
}

int Model::getCurrentTable() const
{
	return currentTable;
}

void Model::setHostAddress(const QString &host)
{
	hostAddress = host;
}

QString Model::getHostAddress() const
{
	return hostAddress;
}

void Model::setTableToJoin(const int &index)
{
	tableToJoin = index;
}

int Model::getTableToJoin() const
{
	return tableToJoin;
}

void Model::setMoneyToRaise(const int &money)
{
	moneyToRaise = money;
}

int Model::getMoneyToRaise() const
{
	return moneyToRaise;
}
