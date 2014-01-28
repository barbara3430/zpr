#ifndef Table_hpp
#define Table_hpp

#include "Player.hpp"

#include <vector>
#include <string>
#include <climits>
#include <ctime>
#include <boost/python.hpp>
#include <Python.h>
#include "json/json.h"

using namespace boost::python;

class Table {
private:
    Table();
    Table(const Table&);
    Table& operator=(const Table&);
    ~Table();

public:

     /**
      * struktura opisujaca stan gry
      */

     struct GameData {
	   //kto ostatni przebijal
       //unsigned last_raise;
           
	   //minimalna stawka na runde
       unsigned turn_min_bet;
	   //stan gry, START = 1; I_BIDDING = 2; CHANGE = 3; II_BIDDING = 4; END = 5;
       unsigned state;
	   //ktory gracz zaczyna, numer jedo siedzenia seat
	   unsigned start_player;
	   //ktorego gracza kolej, numer jedo siedzenia seat
	   unsigned player_turn;
	   //czy trwa rozgrywka
       bool in_game;
	   //ile wynosi all-in w rundzie, potrzebne przy wyliczaniu nagrody
	   unsigned all_in_bet;
	   
	   int player0;
	   int player1;

	unsigned turnMinBet() {
		return turn_min_bet;
	}

	unsigned getState() {
		return state;
	}

	unsigned getStartPlayer() {
		return start_player;
	}

	unsigned getPlayerTurn() {
		return player_turn;
	}

	bool getInGame() {
		return in_game;
	}

	unsigned getAllInBet() {
		return all_in_bet;
	}

	
     };

	/**
      * zwraca GameData stolu
      */
    GameData getGameData() const;
	/**
      * zwraca wektor graczy
      */
    const std::vector<Player>& getPlayers() const;
    /**
      * wylacza stol
      */
    void stopTable();
    /**
      * konstruktor ustawiajacy minimalna stawke na ANTE
      */
      
private:

	 * ustawienie parametrow nowej gry;
	 * zwraca false jesli nie ma dwoch graczy
	 */
	bool newGame();
	/**
	  * proba przebicia przez gracza, arg: nazwa gracza, kwota
	  * kwota musi być wieksza niz game_data.turn_min_bet
	 */
	std::string playerRaise(unsigned seat, unsigned raise);
	/**
	  * proba sprawdzenia przez gracza
	  */
	std::string playerCheck(unsigned seat);
	/**
	  * proba spasowania przez gracza
	  */
	std::string playerFold(unsigned seat);
	/**
	  * proba wejscia za wszystko przez gracza
	  */
	std::string playerAllIn(unsigned seat);
	/**
	  * zakonczenie gry, rozdanie wygranej i ew. wyrzucenie bankrutów,
	  * wyrzuca się tego, ktory nie ma środkow na nastepna gre (musi miec ANTE+1)
	  */
	void endGame();
	/**
	  * sprawdza kto wygral i zapisuje w wektorze winners nr siedzenia (moze byc remis)
	  */
	void getWinners();
	/**
	  * proba dolaczenia do stolu gracza o nazwie name
	 */
	std::string addPlayer(std::string name);
	/**
	  * proba dokonania wymiany kart, 
	  * seat: nr siedzenia (0 lub 1), 
	  * cards: vector z numerami kart do wymiany (od 0 do 51), jesli vector bedzie pusty oznacza, ?e nic nie wymieniac
	  * zwraca false, jesli wymiana wiecej niz 5 lub podano zly numer siedzenia
	 */
	std::string playerChange(unsigned seat, boost::python::list& cards);
	
	std::string updateNames(unsigned s);
		
	std::string updateGame(unsigned s);
	
	//odpowiedz z bledem
	std::string getJsonError(std::string name, std::string info);
	
	//odpowiedz na addPlayer
	std::string addPlayerJson(unsigned seat);
	
	//odpowiedz na updateNames
	std::string refreshNamesJson(unsigned seat);
	
	//odpowiedz na updateGame
	std::string refreshStateJson(unsigned seat, int state);
	
	//odpowiedz na updateGame, jesli koniec gry
	std::string finishGameJson(unsigned s);
	
	//odpowiedz na akcje check, fold, raise
	std::string renderPlayerJson(unsigned s);
	
	//odpowiedz na akcje change 
	std::string setCardsJson(unsigned s);
	
	//odpowiedz na refreshNames
	std::string startGameJson(unsigned s);
	
	
private:

  	GameData game_data;
        std::vector<Player> players; //w tej wersji moze byc tylko dwoch graczy
        std::vector<unsigned> deck; // talia
        unsigned cards_left; //ile kart zostalo w talii
        std::vector<unsigned> winners;

	/**
      * pobranie kolejnej karty z talii
      */
    unsigned pickCard();
    /**
      * potasowanie talii, gdy nowa gra
      */
    void initDeck();
	/**
	  * aktywacja gracza na danym siedzeniu
	  */
	void activate_player(unsigned seat);
	/**
	  * zaznacza, ze dany gracz wykonal akcje
	  */
	void resetActions(unsigned seat);
	/**
	  * sprawdzenie i ew. operacje przy zmianie stanu gry
	  */
	void checkNextState();

public:

    static Table& getInstance() {
        static Table instance;
        return instance;
    }
};

/**
 * rozpoczecie gry; zwraca false, jesli za malo graczy; 
 * W newGame jest pobierane ANTE, stan jest zmieniany na stan nr 2
 */
bool newGame() {
    Table::getInstance().newGame();
}

/**
 * proba przebicia przez gracza, arg: nazwa gracza, kwota
 * kwota musi być wieksza niz game_data.turn_min_bet
 */
std::string playerRaise(unsigned seat, unsigned raise) {
    return Table::getInstance().playerRaise(seat, raise);
}

/**
 * sprawdzenie, kwota jest automatycznie pobierana z konta gracza
 * kwota musi być wieksza niz game_data.turn_min_bet
 */
std::string playerCheck(unsigned seat) {
    return Table::getInstance().playerCheck(seat);
}

std::string playerFold(unsigned seat) {
    return Table::getInstance().playerFold(seat);
}

std::string playerAllIn(unsigned seat) {
    return Table::getInstance().playerAllIn(seat);
}

void getWinners() {
    Table::getInstance().getWinners();
}

std::string addPlayer(std::string n) {
    return Table::getInstance().addPlayer(n);
}

std::string playerChange(unsigned seat, boost::python::list& cards) {
    return Table::getInstance().playerChange(seat, cards);
}

void stopTable() {
    return Table::getInstance().stopTable();
}

Table::GameData  getGameData() {
    return Table::getInstance().getGameData();
}

std::string updateNames(unsigned seat) {
    return Table::getInstance().updateNames(seat);
}

std::string updateGame(unsigned seat) {
    return Table::getInstance().updateGame(seat);
}

BOOST_PYTHON_MODULE(Model)
{
    def("newGame", newGame);
    def("playerRaise", playerRaise);
    def("playerCheck", playerCheck);
    def("playerFold", playerFold);
    def("playerAllIn", playerAllIn);
    def("addPlayer", addPlayer);
    def("playerChange", playerChange);
    def("getGameData", getGameData);
    def("updateNames", updateNames);
    def("updateGame", updateGame);
    def("getWinners", getWinners);        
    def("stopTable", stopTable);          
};

BOOST_PYTHON_MODULE(GameData)
{
    class_<Table::GameData>("GameData")
	.def("turnMinBet", &Table::GameData::turnMinBet)

	.def("getState", &Table::GameData::getState)

	.def("getStartPlayer", &Table::GameData::getStartPlayer)

	.def("getPlayerTurn", &Table::GameData::getPlayerTurn)

	.def("getInGame", &Table::GameData::getPlayerTurn)
	.def("getAllInBet", &Table::GameData::getAllInBet)
    ;
}

#endif

