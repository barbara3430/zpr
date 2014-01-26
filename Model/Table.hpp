#ifndef Table_hpp
#define Table_hpp

#include "Dictionary.hpp"
#include "Player.hpp"

#include <vector>
#include <string>
//#include <queue>
#include <climits>
#include <ctime>

/*#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/utility.hpp>*/

class Table {
public:

      /**
       * struktura opisuj�ca stan gry
       */
       struct GameData {
	    //kto ostatni przebijal
           //unsigned last_raise;
           
	    //minimalna stawka na runde
           unsigned turn_min_bet;
	    //stan gry
           unsigned state;
	   //ktory gracz zaczyna, numer jedo siedzenia seat
	   unsigned start_player;
	   //ktorego gracza kolej, numer jedo siedzenia seat
	   unsigned player_turn;
	   //czy trwa rozgrywka
           bool in_game;
           std::list<unsigned> cards; // HAND_SIZE
        };

	/**
         * zwraca GameData stolu
         */
        GameData getGameData();
        /**
         * wylacza stol
         */
        void stopTable();
        /**
         * konstruktor ustawiajacy minimalna stawke na ANTE
         */
        Table();
        /**
         * destruktor
         */
        ~Table();
private:
        /*boost::thread main_loop;
        boost::mutex queue_mutex;
        boost::shared_mutex data_mutex;
        boost::condition_variable queue_empty;

        std::queue< boost::shared_ptr<TableMessage> > task_queue;*/
        std::vector<Player> players; //w tej wersji moze byc tylko dwoch graczy

	GameData game_data;
        std::vector<unsigned> deck; // talia
        unsigned cards_left; //ile kart zostalo w talii
        std::vector<unsigned> winners;

        //volatile bool running;

	/**
         * pobranie kolejnej karty z talii
         */
        unsigned pickCard();
        /**
         * potasowanie talii
         */
        void initDeck();
	 /**
	  * ustawienie parametrow nowej gry
	  */
	void newGame();
	 /**
	  * aktywacja gracza na konkretnym siedzeniu
	  */
	void activate_player(unsigned seat);
	/**
	  * proba przebicia przez gracza, arg: nr siedzenia (0 lub 1), kwota
	 */
	bool playerRaise(unsigned seat, unsigned raise);
	/**
	  * proba sprawdzenia przez gracza
	  */
	bool playerCheck(unsigned seat);
	/**
	  * proba spasowania przez gracza
	  */
	bool playerFold(unsigned seat);
	/**
	  * proba wejscia za wszystko przez gracza
	  */
	bool playerAllIn(unsigned seat);
	/**
	  * zaznacza, ze dany gracz wykonal akcje
	  */
	void resetActions(unsigned seat);
	/**
	  * sprawdzenie i ew. operacje przy zmianie stanu gry
	  */
	void checkNextState();
	/**
	  * zakonczenie gry
	  */
	void endGame();
	/**
	  * sprawdza kto wygral i zapisuje w wektorze winners nr siedzenia (moze byc remis)
	  */
	void getWinners();
	/**
	  * proba dolaczenia do stolu gracza
	 */
	bool addPlayer();
	/**
	  * proba dokonania wymiany kart, 
	  * seat: nr siedzenia (0 lub 1), 
	  * cards: vector z numerami kart do wymiany (od 0 do 51), jesli vector bedzie pusty oznacza, ?e nic nie wymieniac
	  * zwraca false, jesli wymiana wiecej niz 5 lub podano zly numer siedzenia
	 */
	bool playerChange(unsigned seat, std::vector<unsigned> cards);

};

#endif
