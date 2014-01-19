#pragma once

#include "Connection.hpp"

#include <string>

#include <boost/thread/shared_mutex.hpp>
#include <boost/utility.hpp>

class Account : public boost::noncopyable {
	std::string account_name;
	std::string account_password;
	unsigned account_type;
	unsigned account_cash;
	unsigned table_number;
	bool logged_in;
	ConnectionInfo conn;
	boost::shared_mutex acc_mutex;
	void prv_dbExport(bool, bool);
public:
	/**
	  * konstruktor wzgledem username
	  * pobiera wszystko z bazy danych
	  */
	Account(std::string);
	/**
	  * destruktor
	  */
	~Account();
	/**
	  * zwraca username
	  */
	std::string getName();
	/**
	  * dodaje (odejmuje) kase userowi
	  */
	void addCash(int);
	/**
	  * zwraca stan konta usera
	  */
	unsigned getCash();
	/**
	  * przypisuje usera do stolu
	  */
	void setTable(unsigned);
	/**
	  * zwraca stol na ktorym siedzi user (0 = bez stolu)
	  */
	unsigned getTable();
	/**
	  * zmienia haslo usera
	  */
	void changePassword(std::string);
	/**
	  * weryfikuje haslo usera
	  */
	bool verify(std::string);
	/**
	  * zwraca typ usera, na razie niczym sie nie roznia :)
	  */
	unsigned getType();
	/**
	  * zwraca inf o ostatnim polaczeniu usera
	  */
	ConnectionInfo getConnection();
	/**
	  * aktualizuje ostatnie polaczenie usera
	  */
	void setConnection(const ConnectionInfo&);
	/**
	  * login / logout konta
	  */
	void setLoggedIn(bool);
	/**
	  * sprawdzenie, czy konto jest zalogowane
	  */
	bool isLoggedIn();
};
