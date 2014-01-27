#include "AccountBase.hpp"

#include "Account.hpp"
#include "Database.hpp"
#include "Constants.hpp"
#include "Utility.hpp"

#include <iostream>
#include <vector>

#include <boost/lexical_cast.hpp>

AccountBase::AccountBase()
{
	std::vector< std::vector<std::string> > r;
	r = Database::getInstance().query("SELECT name FROM accounts;");
	for(unsigned i = 0; i < r.size(); ++i) {
		base.insert(std::make_pair(r[i][0],new Account(r[i][0])));
	}
}



bool AccountBase::prv_exists(std::string acc_name)
{
	return base.find(acc_name) != base.end();
}
bool AccountBase::exists(std::string acc_name)
{
	boost::shared_lock<boost::shared_mutex> lock(base_mutex);
	return prv_exists(acc_name);
}
bool AccountBase::prv_verify(std::string acc_name,std::string acc_pass)
{
	if(base.find(acc_name) == base.end()) return false;
	return base.find(acc_name)->second->verify(acc_pass);
}
bool AccountBase::verify(std::string acc_name,std::string acc_pass)
{
	boost::shared_lock<boost::shared_mutex> lock(base_mutex);
	return prv_verify(acc_name,acc_pass);
}

Account& AccountBase::prv_getAccount(std::string acc_name)
{
	assert(base.find(acc_name) != base.end());
	return *(base.find(acc_name)->second);
}
Account& AccountBase::getAccount(std::string acc_name)
{
	boost::shared_lock<boost::shared_mutex> lock(base_mutex);
	return prv_getAccount(acc_name);
}
bool AccountBase::registerAccount(std::string acc_name, std::string acc_pass, unsigned cash, unsigned level)
{
	boost::unique_lock<boost::shared_mutex> lock(base_mutex);
	if(base.find(acc_name) != base.end()) {
				 return false;
	}
	Database::getInstance().query(
				 std::string("INSERT INTO accounts VALUES (\'") +
				 acc_name +
				 "\',\'" +
				 acc_pass +
				 "\',\'" +
				 boost::lexical_cast<std::string>(cash) +
				 "\'," +
				 boost::lexical_cast<std::string>(level) +
				 ");"
	);
	base.insert(std::make_pair(acc_name,new Account(acc_name)));
	return true;
}

bool AccountBase::deleteAccount(std::string acc_name)
{
	boost::unique_lock<boost::shared_mutex> lock(base_mutex);
	if(!prv_exists(acc_name)) {
		return false;
	}
	Database::getInstance().query(
		std::string("DELETE FROM accounts WHERE name = \'") +
		acc_name +
		"\';"
	);
	base.erase(base.find(acc_name));
	return true;
}

AccountBase& AccountBase::getInstance()
{
	static AccountBase instance;
	return instance;
}
