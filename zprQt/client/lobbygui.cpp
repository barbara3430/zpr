#include <map>
#include <stdlib.h>
#include <QString>
#include <QMessageBox>
#include <QStringList>

#include "model.h"
#include "playerinfo.h"
#include "tableinfo.h"
#include "pokerclient.h"
#include "lobbygui.h"
#include "ui_lobbygui.h"
#include "gamewindow.h"

Lobby::Lobby(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Lobby)
{
	ui->setupUi(this);
	Model::getInstance();

	connect(&PokerClient::getInstance(), SIGNAL(joinedTable(bool)), this, SLOT(ifJoinedTable(bool)));
	connect(&PokerClient::getInstance(), SIGNAL(updatedTableList(bool)), this, SLOT(settingUpTablesList(bool)));
	connect(&PokerClient::getInstance(), SIGNAL(updatedUsersList(bool)), this, SLOT(settingUpActiveUsersList(bool)));
	PokerClient::getInstance().requestTablesList();
}

Lobby::~Lobby()
{
	delete ui;
}

void Lobby::on_actionQuit_triggered()
{
	std::cout << "Test\n";
	PokerClient::getInstance().requestLogout();
	QApplication::quit();
}

void Lobby::on_actionAbout_triggered()
{
	QMessageBox *aboutUs = new QMessageBox(QMessageBox::Information, QString("About poker1337"),QString::fromUtf8(
											   "Authors: <ul><li><nobr>Wojciech Szymski</nobr></li>"
											   "<li><nobr>Jakub Borowski</nobr></li>"
											   "<li><nobr>Hubert Świtalski</nobr></li></ul><br />"
											   "modified: 06/03/2012"));
	aboutUs->setIconPixmap(QPixmap(":/img/aboutIcon"));
	aboutUs->setStandardButtons(QMessageBox::Ok);
	aboutUs->setDefaultButton(QMessageBox::Ok);
	aboutUs->exec();
	delete aboutUs;
}

void Lobby::on_quitButton_clicked()
{
	std::cout << "Test\n";
	PokerClient::getInstance().requestLogout();
	QApplication::quit();
}

void Lobby::on_tableInfoButton_clicked()
{
	QModelIndex index;
	on_tablesList_doubleClicked(index);
}

void Lobby::on_sitButton_clicked()
{
	joinTable(ui->tablesList->currentIndex().row());
}

void Lobby::settingUpTablesList(bool ignored)
{
	PokerClient::getInstance().requestAllUsers();
	QStringList *tables = new QStringList;
	for(std::map<int, Tableinfo>::iterator it = Model::getInstance().tableList.begin(); it != Model::getInstance().tableList.end(); it++)
	{
		tables->append(QString("Table no. ") + QString::number(it->second.table_id) + " ( " + QString::number(it->second.buy_in) +
					 " / " + QString::number(it->second.small_blind) + " )");
	}

	QStringListModel *model = new QStringListModel();
	model->setStringList(*tables);

	ui->tablesList->setModel(model);
	ui->tablesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Lobby::settingUpActiveUsersList(bool ignored)
{
	QStringList *players = new QStringList;
	for(std::map<int, Tableinfo>::iterator it = Model::getInstance().tableList.begin(); it != Model::getInstance().tableList.end(); it++)
	{
		Tableinfo t = (*it).second;
		for(std::map<QString, Playerinfo>::iterator it2 = t.players.begin(); it2 != t.players.end(); it2++)
		{
			players->append(it2->second.name);
		}
	}

	QStringListModel *model = new QStringListModel();
	model->setStringList(*players);

	ui->activeUsersList->setModel(model);
	ui->activeUsersList->update();
	ui->activeUsersList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Lobby::on_tablesList_doubleClicked(const QModelIndex &index)
{
	int numSlots = 0;
	QString *playing = new QString;

	for(std::map<QString, Playerinfo>::iterator it = Model::getInstance().tableList[ui->tablesList->currentIndex().row()+1].players.begin(); it != Model::getInstance().tableList[ui->tablesList->currentIndex().row()+1].players.end(); it++)
	{
		playing->append(it->second.name);
		playing->append("<br>");
		numSlots++;
	}

	playing->append(QString("Players: "));
	playing->append(QString::number(numSlots, 10));
	playing->append(QString("/8"));

	int choice;

	QMessageBox *msg = new QMessageBox(QMessageBox::Information, QString("Table info"), *playing);
	msg->setStandardButtons(QMessageBox::Open | QMessageBox::Cancel);
	msg->setDefaultButton(QMessageBox::Open);
	choice = msg->exec();

	switch (choice) {
		case QMessageBox::Open:
			// otwieranie okna ze stoem
			joinTable(ui->tablesList->currentIndex().row());
			delete msg;
			break;
		case QMessageBox::Cancel:
			delete msg;
			break;
		default:
			// should never be reached
			break;
	 }
}

void Lobby::joinTable(int tableID)
{
	Model::getInstance().setTableToJoin(tableID+1);
	PokerClient::getInstance().requestTableJoin();
}

void Lobby::ifJoinedTable(bool)
{
	GameWindow *w = new GameWindow;
	w->show();

}

void Lobby::on_activeUsersList_doubleClicked(const QModelIndex &index)
{
	QString *player = new QString;

//    Player *currentPlayer = new Player();
//    currentPlayer = getPlayerInfo(ui->activeUsersList->currentIndex().row());

	QMessageBox *msg = new QMessageBox(QMessageBox::Information, QString("Player info"), *player);
	msg->setStandardButtons(QMessageBox::Ok);
	msg->setDefaultButton(QMessageBox::Ok);
	msg->exec();
	delete msg;
}

void Lobby::closeEvent(QCloseEvent *event)
{
	std::cout << "Test\n";
	PokerClient::getInstance().requestLogout();
}


void Lobby::on_actionLogout_triggered()
{
	PokerClient::getInstance().requestLogout();
}
