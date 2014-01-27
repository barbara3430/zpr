#pragma once

#include <string>
#include <vector>

#include <boost/thread/mutex.hpp>
#include <boost/utility.hpp>

#include "sqlite/sqlite3.h"

class Database : public boost::noncopyable {
    static std::string db_filename;
    boost::mutex db_mutex;
    sqlite3 *database;
    /**
      * otwarcie polaczenia z dana baza
      */
    bool prv_open(std::string);
    /**
      * zamkniecie polaczenia
      */
    void prv_close();
protected:
    Database();
public:
    /**
      * zapytanie do bazy danych
      */
    std::vector< std::vector<std::string> > query(std::string, bool = false);
    /**
      * WSZEDZIE SINGLETONY!
      */
    static Database& getInstance();
};


