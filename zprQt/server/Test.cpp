#include "Test.hpp"

#include "AccountBase.hpp"
#include "Database.hpp"
#include "Lobby.hpp"
#include "Utility.hpp"

void Test::accountbase()
{
	if(AccountBase::getInstance().exists("test")) {
		assert(AccountBase::getInstance().deleteAccount("test"));
	}
	assert(!AccountBase::getInstance().exists("test"));
	assert(AccountBase::getInstance().registerAccount("test",md5("test"),0,0));
	assert(AccountBase::getInstance().verify("test",md5("test")));
	assert(AccountBase::getInstance().deleteAccount("test"));
	std::cout << (Sync() << "Account test: OK\n");
}

void Test::database()
{
	int uno, dos;
	if(AccountBase::getInstance().exists("test")) {
		AccountBase::getInstance().deleteAccount("test");
	}

	uno = Database::getInstance().query("SELECT name FROM accounts;").size();

	AccountBase::getInstance().registerAccount("test",md5("test"),0,0);

	dos = Database::getInstance().query("SELECT name FROM accounts;").size();

	assert(uno + 1 == dos);

	AccountBase::getInstance().deleteAccount("test");

	uno = Database::getInstance().query("SELECT name FROM accounts;").size();

	assert(uno + 1 == dos);

	std::cout << (Sync() << "Database test: OK\n");
}

void Test::lobby()
{
	std::list<Table::TableDesc> t_list;
	int newtable_id = 1, sizeuno, sizedos;


	t_list = Lobby::getInstance().listTables();
	sizeuno = Lobby::getInstance().listTables().size();

	for(std::list<Table::TableDesc>::iterator it = t_list.begin(); it != t_list.end(); ++it) {
		if(it->table_id != newtable_id) break;
		newtable_id++;
	}

	Lobby::getInstance().createTable(1000,50);

	sizedos = Lobby::getInstance().listTables().size();

	assert(sizeuno + 1 == sizedos);

	Lobby::getInstance().deleteTable(newtable_id);

	sizeuno = Lobby::getInstance().listTables().size();

	assert(sizeuno + 1 == sizedos);

	std::cout << (Sync() << "Lobby test: OK\n");
}
