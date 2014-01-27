QT += core network

TARGET = server
CONFIG += console

TEMPLATE = app

HEADERS += \
	Utility.hpp \
	Test.hpp \
	Table.hpp \
	PokerServer.hpp \
	Player.hpp \
	NetworkQueue.hpp \
	MessageQueue.hpp \
	Message.hpp \
	MD5.hpp \
	Lobby.hpp \
	Hand.hpp \
	Database.hpp \
	Constants.hpp \
	Connection.hpp \
	CommandLine.hpp \
	ClientThread.hpp \
	ClientSocket.hpp \
	AccountBase.hpp \
	Account.hpp \
	sqlite/sqlite3.h


SOURCES += \
	Utility.cpp \
	Test.cpp \
	Table.cpp \
	PokerServer.cpp \
	Player.cpp \
	NetworkQueue.cpp \
	MessageQueue.cpp \
	Message.cpp \
	MD5.cpp \
	main.cpp \
	Lobby.cpp \
	Hand.cpp \
	Database.cpp \
	CommandLine.cpp \
	ClientThread.cpp \
	ClientSocket.cpp \
	AccountBase.cpp \
	Account.cpp \
	sqlite/sqlite3.c \
