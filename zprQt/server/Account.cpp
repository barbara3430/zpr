#include "Account.hpp"

#include "Constants.hpp"
#include "Database.hpp"
#include "Utility.hpp"

#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/thread/locks.hpp>

Account::Account(std::string name)
{
	logged_in = false;
	account_name = name;
	std::vector< std::vector<std::string> > r;
	r = Database::getInstance().query(
		std::string("SELECT pass, cash, level FROM accounts WHERE name = \'") +
		name +
		"\';"
	);
	account_password = r[0][0];
	account_cash = boost::lexical_cast<unsigned>(r[0][1]);
	account_type = boost::lexical_cast<unsigned>(r[0][1]);
	table_number = 0;
#if DEBUGLEV >= 2
	std::cout << (Sync() << "* " << account_name << " (" << account_cash << ") account loaded...\n");
#endif
}
Account::~Account()
{
#if DEBUGLEV >= 2
	std::cout << (Sync() << "* " << account_name << " account unloaded...\n");
#endif
}

void Account::prv_dbExport(bool exp_pass, bool exp_cash)
{
	if(exp_pass) {
		Database::getInstance().query(
			std::string("UPDATE accounts SET password = \'") +
			account_password +
			"\' WHERE name = \'" +
			account_name + "\';"
		);
	}
	if(exp_cash) {
		Database::getInstance().query(
			std::string("UPDATE accounts SET cash = \'") +
			boost::lexical_cast<std::string>(account_cash) +
			"\' WHERE name = \'" +
			account_name +
			"\';"
		);
	}
}
void Account::setTable(unsigned t)
{
	table_number = t;
}
unsigned Account::getTable()
{
	return table_number;
}
void Account::addCash(int bonus_cash)
{
	boost::unique_lock<boost::shared_mutex> lock(acc_mutex);
	account_cash = account_cash + bonus_cash;
	prv_dbExport(false,true);
}
unsigned Account::getCash()
{
	boost::shared_lock<boost::shared_mutex> lock(acc_mutex);
	return account_cash;
}
std::string Account::getName()
{
	boost::shared_lock<boost::shared_mutex> lock(acc_mutex);
	return account_name;
}
void Account::changePassword(std::string pass)
{
	boost::unique_lock<boost::shared_mutex> lock(acc_mutex);
	account_password = pass;
	prv_dbExport(true,false);
}
bool Account::verify(std::string pass)
{
	boost::shared_lock<boost::shared_mutex> lock(acc_mutex);
	return pass == account_password;
}
unsigned Account::getType()
{
	boost::shared_lock<boost::shared_mutex> lock(acc_mutex);
	return account_type;
}
ConnectionInfo Account::getConnection()
{
	boost::shared_lock<boost::shared_mutex> lock(acc_mutex);
	return conn;
}
void Account::setConnection(const ConnectionInfo& c)
{
	boost::unique_lock<boost::shared_mutex> lock(acc_mutex);
	conn = c;
}
void Account::setLoggedIn(bool login)
{
	logged_in = login;
}

bool Account::isLoggedIn()
{
	return logged_in;
}
