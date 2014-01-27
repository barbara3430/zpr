#ifndef Table_hpp
#define Table_hpp

#include "Player.hpp"

#include <vector>
#include <string>
#include <climits>
#include <ctime>
#include <boost/python.hpp>

using namespace boost::python;

class Table {
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
    Table();
    /**
      * destruktor
      */
    ~Table();
  	/**
	 * ustawienie parametrow nowej gry;
	 * zwraca false jesli nie ma dwoch graczy
	 */
	bool newGame();
	/**
	  * proba przebicia przez gracza, arg: nazwa gracza, kwota
	  * kwota musi być wieksza niz game_data.turn_min_bet
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
	bool addPlayer(std::string name);
	/**
	  * proba dokonania wymiany kart, 
	  * seat: nr siedzenia (0 lub 1), 
	  * cards: vector z numerami kart do wymiany (od 0 do 51), jesli vector bedzie pusty oznacza, ?e nic nie wymieniac
	  * zwraca false, jesli wymiana wiecej niz 5 lub podano zly numer siedzenia
	 */
	bool playerChange(unsigned seat, std::vector<unsigned> cards);
	
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

void newGame() {
    Table::getInstance().newGame();
}

bool playerRaise(unsigned seat, unsigned raise) {
    return Table::getInstance().playerRaise(seat, raise);
}

bool playerCheck(unsigned seat) {
    return Table::getInstance().playerCheck(seat);
}

bool playerFold(unsigned seat) {
    return Table::getInstance().playerFold(seat);
}

bool playerAllIn(unsigned seat) {
    return Table::getInstance().playerAllIn(seat);
}

void getWinners() {
    Table::getInstance().getWinners();
}

bool addPlayer(std::string n) {
    return Table::getInstance().addPlayer(n);
}

bool playerChange(unsigned seat, std::vector<unsigned> cards) {
    return Table::getInstance().playerChange(seat, cards);
}

void stopTable() {
    return Table::getInstance().stopTable();
}

GameData  getGameData() {
    return Table::getInstance().getGameData();
}

BOOST_PYTHON_MODULE(Model)
{
    def("newGame", newGame);
    def("playerRaise", playerRaise);
    def("playerCheck", playerCheck);
    def("playerFold", playerFold);
    def("playerAllIn", playerAllIn);
    def("getWinners", getWinners);
    def("addPlayer", addPlayer);
    def("playerChange", playerChange);
	def("stopTable", stopTable);
	def("getGameData", getGameData);
};



BOOST_PYTHON_MODULE(GameData)
{
    class_<GameData>("GameData")
	.def("turnMinBet", &GameData::turnMinBet)

	.def("getState", &GameData::getState)

	.def("getStartPlayer", &GameData::getStartPlayer)

	.def("getPlayerTurn", &GameData::getPlayerTurn)

	.def("getInGame", &GameData::getPlayerTurn)
	.def("getAllInBet", &GameData::getAllInBet)
    ;
}

#endif

