#pragma once

#include "Account.hpp"

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/utility.hpp>

class AccountBase : public boost::noncopyable {
	std::map< std::string, boost::shared_ptr<Account> > base;
	boost::shared_mutex base_mutex;
	bool prv_exists(std::string);
	bool prv_verify(std::string, std::string);
	Account& prv_getAccount(std::string);
protected:
	AccountBase();
public:
	/**
	  * sprawdza czy konto istnieje
	  */
	bool exists(std::string);
	/**
	  * weryfikuje pare accname - password
	  */
	bool verify(std::string, std::string);
	/**
	  * zwraca konto o podanej nazwie
	  */
	Account& getAccount(std::string);
	/**
	  * rejestruje konto (name, pass w md5 male litery, cash, acc_level)
	  */
	bool registerAccount(std::string, std::string, unsigned, unsigned);
	/**
	  * kasuje konto o podanym nicku
	  */
	bool deleteAccount(std::string);
	/**
	  * WSZEDZIE SINGLETONY!
	  */
	static AccountBase& getInstance();
};
