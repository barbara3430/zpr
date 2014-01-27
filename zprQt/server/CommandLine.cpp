#include "CommandLine.hpp"

#include "AccountBase.hpp"
#include "Constants.hpp"
#include "Database.hpp"
#include "Lobby.hpp"
#include "Message.hpp"
#include "MessageQueue.hpp"
#include "NetworkQueue.hpp"
#include "Utility.hpp"

#include <sstream>

CommandLine::CommandLine()
{

}

void CommandLine::run()
{
	while(std::getline(std::cin,command)) {
		 std::string str1,str2;
		 unsigned uint1,uint2;
		 std::stringstream sin(command);
		 if(command.size() == 0) {
			continue;
		 }
		 if(command == "exit" || command == "quit") {
			return;
		 }
		 else if(command.substr(0,9) == "table.add") {
			sin >> str1 /* ignore */ >> uint1 >> uint2;
			Lobby::getInstance().createTable(uint1,uint2);
		 }
		 else if(command.substr(0,9) == "table.del") {
			sin >> str1 /* ignore */ >> uint1;
			Lobby::getInstance().deleteTable(uint1);
		 }
		 else if(command.substr(0,16) == "account.register") {
			sin >> str1 /* ignore */ >> str1 >> str2 >> uint1 >> uint2;
			AccountBase::getInstance().registerAccount(str1,md5(str2),uint1,uint2);
		 }
		 else if(command.substr(0,5) == "table") {
			sin >> str1 /* ignore */ >> uint2 >> uint1 >> str1 >> str2;
			boost::shared_ptr<TableMessage> msg(new TableMessage);
			msg->account_name = str1;
			msg->account_password = md5(str2);
			msg->dest_table = uint2;
			msg->operation = uint1;
			MessageQueueIn::getInstance().addMessage(msg);
		 }
		 else if(command.substr(0,3) == "msg") {
			sin >> str1 /* ignore */ >> uint1 >> str1 >> str2;
			boost::shared_ptr<Message> msg(new Message);
			msg->account_name = str1;
			msg->account_password = md5(str2);
			msg->operation = uint1;
			MessageQueueIn::getInstance().addMessage(msg);
		 }
		 else if(command.substr(0,3) == "sql") {
			std::vector< std::vector<std::string> > q = Database::getInstance().query(command.substr(4,command.size() - 4));
			for(unsigned i = 0; i < q.size(); ++i) {
				for(unsigned j = 0; j < q[i].size(); ++j) {
#if DEBUGLEV >= 0
					std::cout << (Sync() << q[i][j] << " ");
#endif
				}
#if DEBUGLEV >= 0
				std::cout << (Sync() << "\n");
#endif
			}
		 }
		 else {
			sin >> str1 >> str2 >> uint1;
			boost::shared_ptr<Message> msg(new Message);
			msg->account_name = str1;
			msg->account_password = md5(str2);
			msg->uint1 = uint1;
			MessageQueueIn::getInstance().addMessage(msg);
		 }
	}
}
CommandLine& CommandLine::getInstance()
{
	static CommandLine instance;
	return instance;
}
