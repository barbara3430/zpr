#ifndef Player_hpp
#define Player_hpp

#include "Dictionary.hpp"
#include "Hand.hpp"

#include <list>
#include <string>

class Player {
public:
        unsigned cash_available;
        unsigned cash_in_game;
        unsigned bet_this_turn;
        unsigned seat;
	//czy bierze udziel w grze
        bool is_playing;
	//czy jego ruch
        bool is_active;
	//gdy gracz stawia wszystko (kiedy nie ma wystarczajaco duzo by sprawdzic)
        bool all_in;  
        bool left;
	//czy wykonal akcje w rundzie
        bool took_action;
        Hand hand;
        std::string name;
        Player();
        /**
         * konstruktor klasy gracza nr siedzenia - (0 -pierwszy gracz, 1-drugi gracz)
         */
        Player(unsigned);
        /**
         * zaklad gracza (+ aktualizacja maksymalnego zakladu na stole), arg: do zaplacenia, referencja na pole turn_min_bet Player'a
         */
        void bet(unsigned, unsigned&);
};
#endif