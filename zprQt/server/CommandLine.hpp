#pragma once

#include <string>

#include <boost/utility.hpp>

class CommandLine : public boost::noncopyable {
	std::string command;
protected:
	CommandLine();
public:
	/**
	  * glowna petla pobierajaca komendy
	  */
	void run();
	/**
	  * WSZEDZIE SINGLETONY!
	  */
	static CommandLine& getInstance();
};
