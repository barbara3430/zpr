#pragma once

#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include <iostream>
#include <queue>

#include "ui_dialog.h"
/**
 * Klasa odpowiadajaca za polaczenie z serwerem
 */
class PokerClient: public QObject
{
    Q_OBJECT
private:
    PokerClient(QObject* parent = 0);
	/** nazwa uzytkownika */
    QString name;
	/** haslo uzytkownika */
    QString pass;
	/** adres serwera */
    QString host;
	/** Socket odpowiadajacy za transmisje danych */
    QTcpSocket socket;
	/** rozmiar odebranych danych */
    quint16 nextBlockSize;
	/** kolejka zapytan wychodzacych */
    std::queue<std::pair<unsigned, unsigned> > requestsQueue;
	/** kolejka zapytan o informacje o stole */
    std::queue<unsigned> whichTable;
    void closeConnection();
	/**
	 * Metoda odpowiadajaca za wysylanie zapytan do serwera
	 * @param name nazwa uzytkownika
	 * @param pass haslo uzytkownika
	 * @param operation identyfikator operacji
	 * @param dest_table ktorego stolu tyczy sie zapytanie (opcjonalne)
	 * @param uint1 pierwsza wysylana informacja (opcjonalne)
	 * @param uint2 druga wysylana informacja (opcjonalne)
	 * @param uint3 trzecia wysylana informacja (opcjonalne)
	 */
	void sendRequest(QString& name, QString& pass, const unsigned &operation, const unsigned &dest_table, const unsigned uint1, const unsigned uint2, const unsigned uint3);
	/**
	 * Metoda odpowiadajaca za przetworzenie odpowiedzi serwera i wyslanie odpowiedniego sygnalu
	 * @param operation identyfikator operacji
	 * @param id identyfikator wiadomosci
	 * @param uint1 pierwsza odebrana informacja
	 * @param uint2 druga odebrana informacja
	 * @param uint3 trzecia odebrana informacja
	 * @param str1 informacja tekstowa (zazwyczaj nazwa uzytkownika)
	 */
	void processResponse(unsigned &operation, unsigned &id, unsigned &uint1, unsigned &uint2, unsigned &uint3, QString& str1);
public:
	/**
	 * Statyczna metoda zwracajaca obiekt klasy PokerClient
	 */
    static PokerClient& getInstance();
	/**
	 * identyfikator wiadomosci
	 */
    static unsigned messageID;
	/**
	 * Metoda laczaca z serwerem
	 * @return true jezeli polaczenie sie udalo, false w przecinym wypadku
	 */
	void connectToServer();

    //	LOGGING IN
	/**
	 * sprawdza poprawnosc wprowadzonego hasla
	 */
    void checkPassword();
	/**
	 * wylogowywuje z serwera
	 */
	void requestLogout();
    //	LISTNINGS
	/**
	 * zapytanie do serwera o liste stolow
	 */
    void requestTablesList();
	/**
	 * zapytanie do serwera  o liste uzytkownikow przy wybranym stole
	 * @param numer stolu
	 */
    void requestUsersList(int);
	/**
	 * zapytanie do serwera o liste wszystkich uzytkownikow
	 */
    void requestAllUsers();
    //	SINGLE TABLE REQUESTS
	/**
	 * zapytanie do serwera o dolaczenie do stolu
	 */
    void requestTableJoin();
	/**
	 * zapytanie do serwera o odlaczenie od stolu
	 */
    void requestTableLeave();
	/**
	 * zapytanie do serwera o wkupienie ( obecnie wykonywane domyslnie na serwerze )
	 */
    void requestTableBuyIn();
	/**
	 * zapytanie do serwera o zapauzowanie dla danego gracza
	 */
    void requestTablePause();
	/**
	 * zapytanie do serwera o odpauzowanie gracza ( gotowosc do gry )
	 */
    void requestTableUnpause();
    //	COMPLETE TABLE REQESTS
	/**
	 * zapytanie do serwera o podbcie akutalnej stawki
	 */
	void requestTableRaise();
	/**
	 * zapytanie do serwera o wyrownanie lub sprawdzenie
	 */
	void requestTableCheck();
	/**
	 * zapytanie do serwera o spasowanie
	 */
	void requestTableFold();
	/**
	 * zapytanie do serwera o wejscie za wszystko
	 */
	void requestTableAllIn();
public slots:
	/**
	 * Wysyla sygnal o polaczeniu
	 */
	void showConnected();
	/**
	 * Slot zczytujacy dane wyslane przez serwer po wyemitowaniu sygnalu QTcpSocket::readyRead()
	 */
	void receiveData();
	/**
	 * Slot reagujacy na zamkniecie polaczenia przez serwer
	 */
	void connectionClosedByServer();
	/**
	 * Slot reagujacy na blad polaczenia. Zamyka socket
	 */
	void error();
signals:
	/**
	 * Sygnal o poprawnym polaczeniu
	 */
	void connected(bool);
	/**
	 * Sygnal emitowany po odebraniu informacji o zalogowaniu sie
	 */
	void loggedIn(bool);
	/**
	 * Sygnal emitowany po odebraniu informacji o stole
	 */
	void updatedTableList(bool);
	/**
	 * Sygnal emitowany po odebraniu informacji o uzytkowniu
	 */
	void updatedUsersList(bool);
	/**
	 * Sygnal emitowany po odebraniu informacji o dolaczeniu sie do stolu
	 */
	void joinedTable(bool);
	/**
	 * Sygnal emitowany po odebraniu informacji o odlaczeniu sie od stolu
	 */
	void leftTable(bool);
	/**
	 * Sygnal emitowany po odebraniu informacji o wkupieniu sie do gry
	 */
	void boughtIn(bool);
	/**
	 * Sygnal emitowany po odebraniu informacji o zapauzowaniu gry
	 */
	void paused(bool);
	/**
	 * Sygnal emitowany po odebraniu informacji o odpauzowaniu gry
	 */
	void unpaused(bool);
	/**
	 * Sygnal emitowany po odebraniu informacji o podbociu stawki
	 * @param QString kto podbil
	 * @param unsigned do ilu podbil
	 */
	void raised(QString, unsigned);
	/**
	 * Sygnal emitowany po odebraniu informacji o wyrownaniu badz sprawdzeniu
	 * @param QString kto wyrownal/sprawdzil
	 * @param unsigned do ilu wyrownal (lub 0 jezeli sprawdzil)
	 */
	void checked(QString, unsigned);
	/**
	 * Sygnal emitowany po odebraniu informacji o spasowaniu
	 * @param QString kto spasowal
	 * @param unsigned zawsze 0
	 */
	void folded(QString, unsigned);
	/**
	 * Sygnal emitowany po odebraniu informacji o wejsciu za wszystko
	 * @param QString kto wszedl za wszystko
	 * @param unsigned zawsze 0
	 */
	void allIned(QString, unsigned);
	/**
	 * Sygnal emitowany po odebraniu informacji o nowej karcie stolu
	 */
	void newPublicCard();
	/**
	 * Sygnal emitowany po odebraniu informacji o nowej prywatnej karcie
	 */
	void newPrivateCard();
	/**
	 * Sygnal emitowany po odebraniu informacji o nowej karcie zwyciezcy
	 * @param QString czyja karta
	 * @param unsigned wartosc karty (nie wykorzystywana)
	 */
	void newPrivateCard(QString, unsigned);
	/**
	 * Sygnal emitowany po odebraniu informacji o aktywnym graczu
	 * @param kto jest aktywny
	 */
	void activePlayer(QString);
	/**
	 * Sygnal emitowany po odebraniu informacji o nowej rozgrywce
	 */
	void newGame();
};

