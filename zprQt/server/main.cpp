#include "AccountBase.hpp"
#include "CommandLine.hpp"
#include "Lobby.hpp"
#include "MessageQueue.hpp"
#include "NetworkQueue.hpp"
#include "PokerServer.hpp"
#include "Utility.hpp"
#include "Test.hpp"

#include <QApplication>
#include <QMetaType>
#include <QPushButton>
#include <QtCore/QCoreApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	AccountBase::getInstance();
	Lobby::getInstance();
	MessageQueueIn::getInstance();
	MessageQueueOut::getInstance();
	NetworkQueue::getInstance();
	CommandLine::getInstance();
	PokerServer::getInstance();

	if(argc > 1 && std::string(argv[1]) == "--test") {
		Test::accountbase();
		Test::database();
		Test::lobby();
		quit();
	}

	CommandLine::getInstance().run();
	quit();
}
