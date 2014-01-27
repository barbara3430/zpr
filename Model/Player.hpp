#ifndef Player_hpp
#define Player_hpp

#include "Dictionary.hpp"
#include "Hand.hpp"

#include <list>
#include <string>

class Player {
public:
	//nazwa playera jest jego identyfikatorem
        std::string name;
	//nr siedzenia jest tez playera identyfikatorem
	unsigned seat;
	unsigned bet_this_turn;
        unsigned cash_available;
        unsigned cash_in_game;
	//czy bierze udziel w grze
        bool is_playing;
	//czy jego ruch
        bool is_active;
	//gdy gracz stawia wszystko (kiedy nie ma wystarczajaco duzo by sprawdzic)
        bool all_in;  
	//czy wykonal akcje w rundzie
        bool took_action;
        Hand hand;
        Player();
        /**
         * konstruktor klasy; arg: s nr siedzenia gracza(0, 1); n - nazwa gracza
         */
        Player(unsigned s, std::string n);
        /**
         * zaklad gracza (+ aktualizacja maksymalnego zakladu na stole), arg: do zaplacenia, referencja na pole turn_min_bet Player'a
         */
        void bet(unsigned, unsigned&);
	
	void printPlayer() const;
};
#endif