#include "Utility.hpp"

#include "Lobby.hpp"
#include "MessageQueue.hpp"
#include "PokerServer.hpp"

#include <climits>
#include <ctime>
#include <map>

#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>

boost::mt19937 randGen(static_cast<unsigned>(std::time(0)));
boost::uniform_int<> intDist(0, std::numeric_limits<int>::max());
boost::variate_generator< boost::mt19937&, boost::uniform_int<> > GetRand(randGen, intDist);

unsigned Rand()
{
	return static_cast<unsigned>(GetRand());
}

Sync& Sync::operator << (const std::string& x)
{
	out += x;
	return *this;
}
Sync& Sync::operator << (const unsigned& x)
{
	out += boost::lexical_cast<std::string>(x);
	return *this;
}
Sync& Sync::operator << (const char& x)
{
	out += boost::lexical_cast<std::string>(x);
	return *this;
}

std::ostream& operator << (std::ostream& os, const Sync& x)
{
	os << x.out;
	return os;
}

std::string decypherOpCode(unsigned opcode)
{
	std::map<unsigned,std::string> m;
	m[10] = "RETURN_OK";
	m[11] = "RETURN_FAIL";
	m[50] = "LIST_TABLES_RETURNMSG";
	m[51] = "LIST_PLAYERS_RETURNMSG";
	m[60] = "REFRESH_LOBBY";
	m[61] = "REFRESH_TABLE";
	m[106] = "TABLE_NEW_GAME";
	m[110] = "TABLE_RAISE";
	m[111] = "TABLE_CHECK";
	m[112] = "TABLE_FOLD";
	m[113] = "TABLE_ALLIN";
	m[114] = "TABLE_REVEAL_PUBLIC_CARD";
	m[115] = "TABLE_REVEAL_PRIVATE_CARD";
	m[116] = "TABLE_REVEAL_WINNER";
	m[117] = "TABLE_REVEAL_ALLIN";
	m[120] = "TABLE_SET_ACTIVE";
	if(m.find(opcode) == m.end()) {
		return boost::lexical_cast<std::string>(opcode);
	}
	return m[opcode];
}

void quit()
{
	PokerServer::getInstance().stop();
	MessageQueueIn::getInstance().stop();
	MessageQueueOut::getInstance().stop();
	Lobby::getInstance().stop();
	exit(0);
}
