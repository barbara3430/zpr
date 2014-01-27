#pragma once

#include <list>
#include <string>
#include <QString>

/**
 * Klasa przechowuje informacje o graczu
 */
class Playerinfo
{
public:
	/**
	 * Konstuktor klasy.
	 * Przypisuje informacje o zawodniku do odpowidnich pol
	 * @param allCash ilosc dostepnych pieniedzy w danej rozgrywce
	 * @param tableCash kwota postawiona w danej licytacji
	 * @param turn kwota postawiona w danej turze
	 */
	Playerinfo(QString name, unsigned allCash, unsigned tableCash, unsigned turn);
	/** nazwa gracza */
    QString name;
	/** ilosc dostepnych pieniedzy w danej rozgrywce */
    unsigned cash_available;
	/** kwota postawiona w danej licytacji */
    unsigned cash_in_game;
	/** kwota postawiona w danej turze */
    unsigned last_bet;
	/** miejsce przy stole przy ktorym siedzi gracz */
	unsigned seat;
	/**
	 * Domyslny konstuktor, nie powinien byc nigdy uzyty
	 */
    Playerinfo();

};
