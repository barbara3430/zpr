#include "Database.hpp"
#include "Utility.hpp"

#include <boost/thread/locks.hpp>

Database::Database()
{

}

bool Database::prv_open(std::string filename)
{
	return sqlite3_open(filename.c_str(), &database) == SQLITE_OK;
}

std::vector< std::vector<std::string> > Database::query(std::string query, bool print)
{
	boost::lock_guard<boost::mutex> lock(db_mutex);
	std::vector< std::vector<std::string> > results;
	if(!prv_open(db_filename)) {
		std::string error = sqlite3_errmsg(database);
#if DEBUGLEV >= 0
		std::cout << (Sync() << query << " " << error << "\n");
#endif
		results.clear();
		return results;
	}
	if(print) {
#if DEBUGLEV >= 0
		std::cout << (Sync() << query << "\n");
#endif
	}
	sqlite3_stmt *statement;


	if(sqlite3_prepare_v2(database, query.c_str(), -1, &statement, 0) == SQLITE_OK)
	{
		int cols = sqlite3_column_count(statement);
		int result = 0;
		while(true)
		{
			result = sqlite3_step(statement);

			if(result == SQLITE_ROW)
			{
				std::vector<std::string> values;
				for(int col = 0; col < cols; col++)
				{
					values.push_back((char*)sqlite3_column_text(statement, col));
				}
				results.push_back(values);
			}
			else
			{
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	std::string error = sqlite3_errmsg(database);
	if(error != "not an error") {
#if DEBUGLEV >= 0
		std::cout << (Sync() << query << " " << error << "\n");
#endif
		results.clear();
	}
	prv_close();
	return results;
}

void Database::prv_close()
{
	sqlite3_close(database);
}

Database& Database::getInstance()
{
	static Database instance;
	return instance;
}

std::string Database::db_filename = "Database.db";
