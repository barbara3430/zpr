#ifndef Hand_hpp
#define Hand_hpp

#include "Card.hpp"

#include <list>
#include <vector>

class Hand {
	unsigned type; //jaki uklad na rece
	std::vector<Card> cards; //karty w rece
	
	/**
	 * sprawdzenie ukladu kart, ustawienie type
	 */
	void checkHand();
	void checkRoyalFlush();
	void checkStraightFlush();
	void checkFour(std::vector<int>&);
	void checkFull(std::vector<int>&);
	void checkFlush();
	void checkStraight();
	void checkThree(std::vector<int>&);
	void checkTwoPair(std::vector<int>&);
	void checkPair(std::vector<int>&);
	/**
	 * czy karty w rece sa jednego koloru,
	 * pomocnicza funkcja przy ustalaniu ukladu reki
	 */	
	bool isMonoColor();
	/**
	 * czy w rece jest 5 nastepujacych po sobie kart,
	 * pomocnicza funkcja przy ustalaniu ukladu reki
	 */	
	bool isAsc();
	/**
	 * zwraca najwyzsza karte w rece
	 */
	Card getMaxCard() const;
	
public:
	Hand();
	/**
	  * ustawia reke kartami z zadanego wektora
	  */
	void set(const std::vector<unsigned>);
	/**
	  * do porownywania sily rak
	  * w przypadku dwoch wysokich rak porownywane sa najwyzsze karty z reki
	  */
	bool operator < (const Hand&) const;
	/**
	  * czy na rece jest ten sam układ
	  */
	bool operator == (const Hand&) const;
	/**
	  * zwraca uklad na rece
	  */
	unsigned getType() const;
	/**
	  * zwraca wektor kart (ich numerow (0-51))
	  */
	std::vector<unsigned> getCards() const;
};

#endif