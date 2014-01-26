#pragma once

#include "Constants.hpp"
#include "Message.hpp"
#include "Player.hpp"

#include <map>
#include <string>
#include <queue>
#include <vector>

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/utility.hpp>

class Table : public boost::noncopyable {
public:
	/**
	* wewnetrzna struktura opisujaca stol widziany z poziomu lobby
	*/
	struct TableDesc {
		unsigned buy_in;
		unsigned small_blind;
		unsigned table_id;
		unsigned game_nr;
	};
	/**
	* wewnetrzna struktura opisujaca stol widziany z poziomu gracza
	*/
	struct GameData {
		unsigned last_raise;
		unsigned turn_min_bet;
		unsigned dealer;
		unsigned state;
		bool in_game;
		std::list<unsigned> cards; // HAND_SIZE
	};
private:
	boost::thread main_loop;
	boost::mutex queue_mutex;
	boost::shared_mutex data_mutex;
	boost::condition_variable queue_empty;

	std::queue< boost::shared_ptr<TableMessage> > task_queue;
	std::map<std::string,Player> players;

	std::vector<unsigned> deck; // DECK_SIZE
	unsigned cards_left;
	std::vector<std::string> winner_table;

	TableDesc table_desc;
	GameData game_data;
	volatile bool running;

	/**
	  * glowna petla dla zadan stolu
	  */
	void prv_mainLoop();
	/**
	  * zmielenie pojedynczej operacji
	  */
	void prv_processOperation(boost::shared_ptr<TableMessage>);
	/**
	  * zamiana numeru siedzenia na iterator mapy graczy
	  */
	std::map<std::string,Player>::iterator prv_seatToIterator(unsigned);
	/**
	  * wybor nastepnego gracza, ktory moze licytowac
	  */
	std::map<std::string,Player>::iterator prv_getNextPlayer(unsigned);
	/**
	  * aktywacja nastepnego gracza wzgledem siedzen
	  */
	//void prv_activateNext();
	/**
	  * aktywacja gracza na konkretnym siedzeniu
	  */
	//void prv_activate(unsigned);
	/**
	  * pobranie numeru siedzenia obecnie aktywnego gracza
	  */
	unsigned prv_activeNowSeat();
	/**
	  * pobranie imienia obecnie aktywnego gracza
	  */
	std::string prv_activeNowName();
	/**
	  * proba dolaczenia do stolu gracza
	  */
	bool prv_joinTable(std::string);
	/**
	  * proba odlaczenia do stolu gracza
	  * jesli jest w trakcie gry zostaje po nim "fantom" do konca danej gry
	  */
	bool prv_leaveTable(std::string);
	/**
	  * czy gracz jest na tym stole
	  */
	bool prv_isAtTable(std::string);
	/**
	  * wywolanie nowej gry
	  */
	//void prv_newGame();
	/**
	  * sprawdzenie i ew. operacje przy zmianie stanu gry
	  */
	//void prv_checkNextTurn();
	/**
	  * stworzenie tabeli wygranych
	  */
	void prv_createWinnerTable();
	/**
	  * komparator do porownywania rak dwoch graczy
	  */
	bool prv_playerComparator(std::string, std::string);
	/**
	  * pobranie wektora wygranych
	  */
	std::vector<std::string> prv_getWinners();
	/**
	  * operacje zwiazane z koncem gry
	  */
	//void prv_endGame();
	/**
	  * proba wkupienia sie gracza
	  */
	bool prv_playerBuyIn(std::string);
	/**
	  * zwraca numer siedzenia dla nowego gracza
	  */
	unsigned prv_newSeat();
	/**
	  * proba oznaczenia sie jako gotowy/niegotowy
	  */
	bool prv_playerSetPause(std::string, bool);
	/**
	  * wewnetrzna wiadomosc, triggeruje sprawdzenie czy powinna sie rozpoczac gra
	  */
	void prv_sendCheckForNewGame();
	/**
	  * sprawdzenie czy powinna sie rozpoczac gra
	  */
	void prv_checkForNewGame();
	/**
	  * pobranie kolejnej karty z talii
	  */
	//unsigned prv_pickCard();
	/**
	  * potasowanie talii
	  */
	//void prv_initDeck();
	/**
	  * proba podbicia przez gracza
	  */
	//bool prv_playerRaise(std::string, unsigned);
	/**
	  * proba sprawdzenia przez gracza
	  */
	//bool prv_playerCheck(std::string);
	/**
	  * proba spasowania przez gracza
	  */
	//bool prv_playerFold(std::string);
	/**
	  * proba wejscia za wszystko przez gracza
	  */
	//bool prv_playerAllIn(std::string);
	/**
	  * resetuje sprawdzanie akcji gdy ktos podbije
	  */
	//void prv_resetActions(unsigned);
	/**
	  * wysyla wszystkim karte ze stolu
	  */
	void prv_revealTableCard(unsigned);
	/**
	  * wysyla graczom wlasne karty
	  */
	void prv_revealOwnCard(std::string, unsigned);
	/**
	  * wysyla wszystkim karty wygranych / allinow (
	  */
	void prv_revealPrivateCardToAll(std::string, unsigned, unsigned);
	/**
	  * wyslanie wiadomosci do konkretnego gracza
	  */
	void prv_sendToOne(std::string, unsigned, std::string, unsigned, unsigned, unsigned, unsigned);
	/**
	  * wyslanie wiadomosci do wszystkich na stole
	  */
	void prv_sendToAll(unsigned, std::string, unsigned, unsigned, unsigned, unsigned);

public:
	/**
	  * zwraca GameData danego stolu
	  */
	GameData getGameData();
	/**
	  * zwraca TableDesc danego stolu
	  */
	TableDesc getTableDesc();
	/**
	  * dodaje TableMessage do kolejki zadan
	  */
	void addTask(boost::shared_ptr<TableMessage>);
	/**
	  * wylacza stol
	  */
	void stopTable();
	/**
	  * konstruktor z parametrami id, wejscie, maly blind
	  */
	Table(unsigned, unsigned, unsigned);
	/**
	  * destruktor
	  */
	~Table();

};
