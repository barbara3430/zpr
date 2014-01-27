#include "Card.hpp"

Card::Card(unsigned c)
{
	card = c;
}

unsigned Card::getC() const
{
	return card / 13; // 0 trefl 1 karo 2 kier 3 pik
}

unsigned Card::getV() const
{
	return card % 13; // 0 2, 1 3, ..., 8 10, 9 J, 10 Q, 11 K, 12 A
}

unsigned Card::getCard() const
{
	return card;
}

std::string Card::print() const
{
	std::string ret;
	return ret + "234567890JQKA"[getV()] + ' ' + "TKaKieP"[getC()];
}

bool Card::operator < (const Card& c) const
{
	if(getV() != c.getV()) {
		return getV() > c.getV();
	}
	return getC() > c.getC();
}

bool Card::operator == (const Card& c) const
{
	return getCard() == c.getCard();
}