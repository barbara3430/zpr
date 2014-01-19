#pragma once

#ifndef DEBUGLEV
#define DEBUGLEV 4
#endif

#include "MD5.hpp"
#include "Test.hpp"

#include <iostream>

/**
 * Przejrzysta nakladka na boostowy random
 */
unsigned Rand();

/**
 * klasa sluzaca do synchronizacji wyjscia
 * cout << (Sync() << ...);
 */
class Sync
{
	std::string out;
public:
	Sync& operator << (const std::string&);
	Sync& operator << (const unsigned&);
	Sync& operator << (const char&);
	friend std::ostream& operator << (std::ostream&, const Sync&);
};
std::ostream& operator << (std::ostream&, const Sync&);


/**
 * zamiana opcode na stringa
 */
std::string decypherOpCode(unsigned);

void quit();
