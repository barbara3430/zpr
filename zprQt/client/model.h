#pragma once

#include <map>

#include "tableinfo.h"
#include "playerinfo.h"

class Model
{
    QString userName;					// account_name of current user
    QString userPass;					// account_pass of current user
    QString hostAddress;				// network address
    int tableToJoin;
    int moneyToRaise;
protected:
    Model();
public:
	/** Numer stolu na ktorym aktualnie toczy sie gra */
	int currentTable;
	/** Publiczne karty danego stolu */
    std::vector<unsigned> cards;
	/** Mapa wszystkich stolow, identyfikowanych numerem */
    std::map<int, Tableinfo> tableList;	// list of all tables and users on them
	/** Statyczna metoda zwracajaca obiekt Singleton */
	static Model& getInstance();
	/** Metoda ustawiajaca nazwe zalogowanego uzytkownika
	 * @param name nazwa uzytkownika
	 */
    void setUserName(const QString& name);
	/**
	 * Metoda zwracajaca nazwe aktualnie zalogowanego uzytkownika
	 */
	QString getUserName() const;
	/**
	 * Metoda ustawiajaca haslo zalogowanego uzytkownika
	 * @param pass haslo uzytkownika
	 */
	void setUserPass(const QString& pass);
	/**
	 * Metoda zwracajaca haslo zalogowanego uzytkownika
	 */
	QString getUserPass() const;
	/**
	 * Metoda czyszczaca mape stolow
	 */
	void clearTableList();
	/**
	 * Metoda wstawiajaca informacje o jednym stole do mapy
	 * @param table informacje o stole
	 */
	void addOneTable(Tableinfo table);
	/**
	 * Metoda czyszczaca informacje o uzytkownikach przy wybranym stole
	 * @param tableNumber numer stolu do wyczyszczenia
	 */
    void clearUsersOnTable(int tableNumber);
	/**
	 * Metoda dodajaca informacje o jednym uzytkowniku do wybranego stolu
	 * @param tableNumber numer stolu
	 * @param player informacje o graczu
	 */
	void upadateUsersOnTable(int tableNumber, Playerinfo player);
	/**
	 * Ustawienie aktualnego stolu na zadany
	 * @param numer stolu do ustawienia
	 */
	void setCurrentTable(const int& tableNumber);
	/**
	 * zwraca numer aktualnego stolu
	 */
	int getCurrentTable() const;
	/**
	 * ustawia adres serwera
	 * @param host adres serwera
	 */
    void setHostAddress(const QString &host);
	/**
	 * zwraca adres serwera
	 */
    QString getHostAddress() const;
	/**
	 * ustawia numer stolu do ktorego uzytkownik chce sie dolaczyc na zadany
	 * @param index number stolu
	 */
    void setTableToJoin(const int &index);
	/**
	 * zwraca numer stolu do ktorego uzytkownik chce sie dolaczyc
	 */
    int getTableToJoin() const;
	/**
	 * ustawia wysokosc kwoty do ktorej uzytkownik chce podbic stawke
	 * @param money kwota
	 */
    void setMoneyToRaise(const int &money);
	/**
	 * zwraca kwote do podbicia
	 */
    int getMoneyToRaise() const;
};
