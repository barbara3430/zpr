#include "Lobby.hpp"

#include "AccountBase.hpp"
#include "Database.hpp"
#include "Table.hpp"
#include "Utility.hpp"

#include <boost/lexical_cast.hpp>


Lobby::Lobby()
{
	std::vector< std::vector<std::string> > r;
	r = Database::getInstance().query("SELECT * FROM tables;");
	for(unsigned i = 0; i < r.size(); ++i) {
		tables.push_back(boost::shared_ptr<Table>(new Table(
			boost::lexical_cast<unsigned>(r[i][0]),
			boost::lexical_cast<unsigned>(r[i][1]),
			boost::lexical_cast<unsigned>(r[i][2])
		)));
	}
}
Lobby::~Lobby()
{
	stop();
}

void Lobby::sendMessage(boost::shared_ptr<TableMessage> msg)
{
	boost::shared_lock<boost::shared_mutex> lock(tables_mutex);
	for(std::list< boost::shared_ptr<Table> >::iterator it = tables.begin(); it != tables.end(); ++it) {
		if((*it)->getTableDesc().table_id == msg->dest_table) {
			(*it)->addTask(msg);
			return;
		}
	}
	msg->return_msg(false);
}
bool Lobby::createTable(unsigned buyin, unsigned sblind)
{
	boost::unique_lock<boost::shared_mutex> lock(tables_mutex);
	unsigned new_id = 1;
	std::list< boost::shared_ptr<Table> >::iterator it;
	for(it = tables.begin(); it != tables.end(); ++it, ++new_id) {
		if((*it)->getTableDesc().table_id != new_id) break;
	}
	tables.insert(it,boost::shared_ptr<Table>(new Table(new_id,buyin,sblind)));
	return true;
}

bool Lobby::deleteTable(unsigned table_id)
{
	boost::unique_lock<boost::shared_mutex> lock(tables_mutex);
	for(std::list< boost::shared_ptr<Table> >::iterator it = tables.begin(); it != tables.end(); ++it) {
		if((*it)->getTableDesc().table_id == table_id) {
			tables.erase(it);

			return true;
		}
	}
	return false;
}
void Lobby::stop()
{
	boost::unique_lock<boost::shared_mutex> lock(tables_mutex);
	if(tables.size() > 0) {
		Database::getInstance().query("DELETE FROM tables;");
	}
	while(tables.size()) {
		Database::getInstance().query(
			std::string("INSERT INTO tables VALUES (") +
			boost::lexical_cast<std::string>(tables.front()->getTableDesc().table_id) + ", " +
			boost::lexical_cast<std::string>(tables.front()->getTableDesc().buy_in) + ", " +
			boost::lexical_cast<std::string>(tables.front()->getTableDesc().small_blind) + ");"
		);
		tables.erase(tables.begin());
	}
}
Lobby& Lobby::getInstance()
{
	static Lobby instance;
	return instance;
}

std::list<Table::TableDesc> Lobby::listTables()
{
	boost::shared_lock<boost::shared_mutex> lock(tables_mutex);
	std::list<Table::TableDesc> ret;
	for(std::list< boost::shared_ptr<Table> >::iterator it = tables.begin(); it != tables.end(); ++it) {
		ret.push_back((*it)->getTableDesc());
	}
	return ret;
}
