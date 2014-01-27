#ifndef Card_hpp
#define Card_hpp

#include "Dictionary.hpp"
#include <string>

class Card {
	unsigned card;
public:
	unsigned getC() const; //zwróć kolor
	unsigned getV() const; //zwróć figurę
	unsigned getCard() const; //zwróć numer kart
	std::string print() const;
	Card(unsigned);
	
	bool operator < (const Card&) const;
	bool operator == (const Card&) const;
};

#endif