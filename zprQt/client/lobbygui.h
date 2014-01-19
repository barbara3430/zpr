#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QtGui>
#include <map>

#include "tableinfo.h"
#include "playerinfo.h"
#include "model.h"

namespace Ui {
class Lobby;
}
/**
 * Klasa wyswietlajaca Lobby ( liste stolow, liste aktywnych uzytkownikow)
 */
class Lobby : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * Konstuktor klasy Lobby. Tworzy okno i laczy odpowiednie sygnaly ze slotami
	 */
	explicit Lobby(QWidget *parent = 0);
	/**
	 * Destruktor klasy lobby
	 */
	~Lobby();
	/**
	 * Metoda odpowiadajaca za wylogowanie podczas wylaczania
	 */
	void closeEvent(QCloseEvent *event);
private slots:
	void on_actionQuit_triggered();
	void on_actionAbout_triggered();
	void on_quitButton_clicked();
	void on_tableInfoButton_clicked();
	void on_sitButton_clicked();
	void on_tablesList_doubleClicked(const QModelIndex &index);
	void on_activeUsersList_doubleClicked(const QModelIndex &index);
	void ifJoinedTable(bool);
	void settingUpTablesList(bool);
	void settingUpActiveUsersList(bool);
	void on_actionLogout_triggered();

private:
	Ui::Lobby *ui;
	void joinTable(int);
};
