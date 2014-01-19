#pragma once

#include "Constants.hpp"
#include "Hand.hpp"

#include <list>
#include <string>

class Player {
public:
	unsigned cash_available;
	unsigned cash_in_game;
	unsigned bet_this_turn;
	std::list<unsigned> card;
	unsigned seat;
	bool is_playing;
	bool is_active;
	bool all_in;
	bool pause;
	bool rewarded;
	bool current_winner;
	bool left;
	bool took_action;
	bool revealed;
	Hand hand;
	std::string name;
	Player();
	/**
	  * konstruktor klasy gracza (nr siedzenia, imie)
	  */
	Player(unsigned,std::string);
	/**
	  * zaklad gracza (+ aktualizacja maksymalnego zakladu na stole)
	  */
	void bet(unsigned, unsigned&);
};
