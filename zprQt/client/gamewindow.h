#pragma once

#include <QMainWindow>
#include <QLabel>

#include "tableinfo.h"

namespace Ui {
    class GameWindow;
}
/**
 * Klasa odpowiadajaca za wyswietlanie stolu, i przebiegu rozgrywki
 */
class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
	/**
	 * Konsturktor okna gry
	 */
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

	/**
	 * Metoda wyswietlajaca QDialog z zapytaniem, czy uzytkownik jest gotowy do gry
	 */
	bool checkReady();
	/**
	 * Metoda odpowiadajaca za wyslanie do serwera informacji, ze uzytkownik odszedl od stolu, jezeli zamknal okno
	 */
	void closeEvent(QCloseEvent *event);
	/**
	 * Metoda odpowiadajaca za odswiezenie informacji o stole
	 */
	void updateTableInfo();
private slots:
	/**
	 * Slot odpowiadajacy za przygotowanie i wyswietlenie przyciskow podczas tworzenia okna
	 */
    void prepareButtons();
	/**
	 * Slot ustawiajacy okreslone wartosci spinbox i slider
	 * @param step o ile zmieni sie wartosc w pojedynczym przesunieciu
	 * @param max jaka jest maksymalna wartosc do postawienia
	 * @param min jaka jest minimalna wartosc do postawienia
	 */
	void prepareSliderAndSpinBox(unsigned step, unsigned max, unsigned min);
	/**
	 * Slot wyswietlajacy przy nicku gracza jego obecna kwote pieniedzy, oraz aktualizujacy pule
	 */
	void updateCash();
	/**
	 * Slot aktualizujacy graczy przy stole, oraz ich karty
	 */
    void updatePlayers();
	/**
	 * Slot aktualizujacy informacje wyswietlane na przyciskach, oraz wartosci Spinbox'a i Slidera
	 * @param name informacja od serwera, ktory gracz ma teraz ruch
	 */
    void updateButtons(QString name);
	/**
	 * Slot sluzacy do zsynchronizowania Spinbox'a i Slidera
	 */
    void changeBetSpinBoxValue(int value);
	/**
	 * Slot sluzacy do zsynchronizowania Spinbox'a i Slidera
	 */
    void changeBetSliderValue(int value);
	/**
	 * Slot wysylajacy do serwera zapytanie o podbicie stawki
	 */
    void playerBetRaise();
	/**
	 * Slot wysylajacy do serwera zapytanie o wyrownanie badz sprawdznie
	 */
    void playerCallCheck();
	/**
	 * Slot wysylajacy do serwera zapytanie o spasowanie
	 */
    void playerFold();
	/**
	 * Slot wysylajacy do serwera zapytanie o wejscie za wszelkie dostepne pieniadze
	 */
    void playerAllIn();
	/**
	 * Slot wyswietlajacy prywatne karty
	 */
    void updatePrivateCard();
	/**
	 * Slot wyswietlajacy zwyciezce, oraz zerujacy pule
	 * @param who kto wygral
	 * @param card jaka byla jego karta
	 */
	void updatePrivateCard(QString who, unsigned card);
	/**
	 * Slot wystietlajacy publiczne karty
	 */
	void updatePublicCard();
	/**
	 * Slot odpowiadajacy za odswiezenie wyswietlanych informacji
	 */
	void update();
	/**
	 * Slot odpowiadajacy za przygotowanie nowej gry, wyczyszczenie kart wspolnych.
	 */
	void prepareNewGame();
	/**
	 * Slot odpowiadajacy za obliczenia zawiazane z zakladami
	 */
	void calculateRaise(QString, unsigned);
	/**
	 * Slot odpowiadajacy za obliczenia zwiazane z wejsciem za wszystko
	 */
	void calculateAllIn(QString name, unsigned money);
private:
	/** obiekt okna */
    Ui::GameWindow *ui;
	/** numer aktualnie otwartego stolu */
	int currentTable;
	/** miejsce przy stole na ktorym siedzi gracz */
	int userSeat;
	/** laczna wartosc zakladow */
	unsigned pot;
	/** tablica wskaznikow do etykiet z nazwami graczy */
	QLabel *players[8];
	/** tablica wskaznikow do etykiet z iloscia pieniedzy graczy */
	QLabel *cash[8];
	/** tablica wskaznikow do etykiet z kartami graczy */
	QLabel *cards[8][2];
	/** przypisanie wskaznikow */
	void createPointers();
};
