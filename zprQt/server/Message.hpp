#pragma once

#include "Connection.hpp"

#include <string>

class Message {
protected:
	unsigned msg_type;
public:
	Message();
	virtual ~Message();
	unsigned getType();
	/**
	  * generowanie odpowiedzi na wiadomosc wejsciowa
	  */
	virtual void return_msg(bool);

	std::string account_name;
	std::string account_password;
	unsigned operation;
	unsigned id;
	unsigned uint1,uint2,uint3;
	std::string str1;
	bool no_return;
	ConnectionInfo conn;
};


class TableMessage : public Message {
public:
	TableMessage();
	~TableMessage();
	/**
	  * konstruktor do testow
	  */
	TableMessage(std::string, std::string, unsigned, unsigned);
	unsigned dest_table;
};

