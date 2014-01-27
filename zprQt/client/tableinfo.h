#pragma once

#include <map>
#include <vector>
#include <string>
#include <list>

#include "playerinfo.h"

/**
 * Klasa przechowujaca informacje o stole
 */
class Tableinfo
{
public:
	/**
	 * Domyslny konstruktor (nie powinien zostac uzyty)
	 */
    Tableinfo();
	/**
	 * Konstruktor inicjujacy wartosci zwracane przez serwer
	 * @param id numer stolu
	 * @param buyin kwota wkupienia
	 * @param smallblind mala ciemna
	 */
    Tableinfo(int id, int buyin, int smallblind);
	/** numer stolu */
    unsigned table_id;
	/** kwota wkupienia */
    unsigned buy_in;
	/** mala ciemna */
	unsigned small_blind;

	unsigned dealer;
	/** Kto wykonal ostatnia akcje */
	QString last_player;
	/** Jaka byla ostatnia akcja */
	unsigned last_action;
	/** Jezeli ostatnia akcja bylo pobicie to o ile */
	unsigned last_bet;
	/** Karty stolu */
    std::vector<unsigned> table_cards;
	/** mapa graczy przy stole */
    std::map<QString,Playerinfo> players;
	/** Lista graczy przy stole, ustawieni w kolejnosci rozdawania */
	std::list<QString> playersQueue;

	/** Stala opisujaca akcje podbicia */
	static const unsigned RAISED = 1;
	/** Stala opisujaca akcje wyrownania lub sprawdzenia */
	static const unsigned CHECKED = 2;
	/** Stala opisujaca akcje spasowania */
	static const unsigned FOLDED = 3;
	/** Stala opisujaca akcje wejscia za wszystko */
	static const unsigned ALLINED = 4;
	/**
	 * Metoda ustawiajaca ostatnia akcje
	 * @param action ostatnia akcja
	 */
	void setLastAction(int action);
};
