#pragma once

#include "Constants.hpp"

#include <list>
#include <string>
#include <vector>

class Card {
	unsigned card;
public:
	unsigned c() const;
	unsigned v() const;
	std::string print() const;
	Card(unsigned);
	/**
	  * porownanie dwoch kart
	  */
	bool operator < (const Card&) const;
};

class Hand {
	unsigned type;
	unsigned cr[CRITERIA];
	std::vector<Card> card;
	/**
	  * obliczenie najsilniejszego zestawu 5 z 7 kart
	  */
	void calculateHand();
	/**
	  * podwykonawca sprawdzajacy czy dany podzestaw jest danym ukladem
	  */
	void checkHand(const std::vector<Card>&, unsigned, unsigned, unsigned(const std::vector<Card>&,unsigned));
	static unsigned checkRoyalFlush(const std::vector<Card>&,unsigned);
	static unsigned checkStraightFlush(const std::vector<Card>&,unsigned);
	static unsigned checkFour(const std::vector<Card>&,unsigned);
	static unsigned checkFull(const std::vector<Card>&,unsigned);
	static unsigned checkFlush(const std::vector<Card>&,unsigned);
	static unsigned checkStraight(const std::vector<Card>&,unsigned);
	static unsigned checkThree(const std::vector<Card>&,unsigned);
	static unsigned checkTwoPair(const std::vector<Card>&,unsigned);
	static unsigned checkPair(const std::vector<Card>&,unsigned);
	static unsigned checkHighCard(const std::vector<Card>&,unsigned);
public:
	Hand();
	/**
	  * pobiera karty z listy kart na rece i listy kart na stole
	  */
	void set(const std::list<unsigned>&, const std::list<unsigned>&);
	/**
	  * do porownywania sily rak
	  */
	bool operator < (const Hand&) const;
	/**
	  * do porownywania sily rak
	  */
	bool operator == (const Hand&) const;
	/**
	  * zwraca uklad na rece
	  */
	unsigned getType();
	/**
	  * poboczne kryteria sily ukladu
	  */
	unsigned getCriteria(unsigned);
};
