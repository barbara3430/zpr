#pragma once

#include "Table.hpp"

#include <list>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/utility.hpp>

class TableMessage;

class Lobby : public boost::noncopyable {
	boost::shared_mutex tables_mutex;
	std::list<std::string> players;
	std::list< boost::shared_ptr<Table> > tables;

protected:
	Lobby();
public:
	/**
	  * destruktor
	  */
	~Lobby();
	/**
	  * tworzy stol o podanym wejsciu i malym blindzie
	  */
	bool createTable(unsigned,unsigned);
	/**
	  * kasuje stol o danym id
	  */
	bool deleteTable(unsigned);
	/**
	  * wysyla wiadomosc do stolu danego w wiadomosci
	  */
	void sendMessage(boost::shared_ptr<TableMessage>);
	/**
	  * zwraca liste opisow stolow
	  */
	std::list<Table::TableDesc> listTables();
	/**
	  * zatrzymuje wszystkie stoly
	  */
	void stop();
	/**
	  * WSZEDZIE SINGLETONY!
	  */
	static Lobby& getInstance();
};
