#include <QPalette>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QStringListModel>
#include <QThread>

#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "tableinfo.h"
#include "playerinfo.h"
#include "pokerclient.h"
#include "model.h"


GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GameWindow), userSeat(-1), pot(0)
{
    ui->setupUi(this);
	createPointers();
	currentTable = Model::getInstance().currentTable;

	//UI connections
    connect(ui->betRaiseButton, SIGNAL(clicked()), this, SLOT(playerBetRaise()));
    connect(ui->callCheckButton, SIGNAL(clicked()), this, SLOT(playerCallCheck()));
    connect(ui->foldButton, SIGNAL(clicked()), this, SLOT(playerFold()));
    connect(ui->allInButton, SIGNAL(clicked()), this, SLOT(playerAllIn()));
    connect(ui->betSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBetSpinBoxValue(int)));
    connect(ui->betSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeBetSliderValue(int)));

	// game connections
    connect(&PokerClient::getInstance(), SIGNAL(newPrivateCard()), this, SLOT(updatePrivateCard()));
	connect(&PokerClient::getInstance(), SIGNAL(newPrivateCard(QString, unsigned)), this, SLOT(updatePrivateCard(QString,unsigned)));
	connect(&PokerClient::getInstance(), SIGNAL(raised(QString, unsigned)), this, SLOT(calculateRaise(QString, unsigned)));
	connect(&PokerClient::getInstance(), SIGNAL(checked(QString, unsigned)), this, SLOT(calculateRaise(QString, unsigned)));
	connect(&PokerClient::getInstance(), SIGNAL(folded(QString,unsigned)), this, SLOT(update()));
	connect(&PokerClient::getInstance(), SIGNAL(allIned(QString,unsigned)), this, SLOT(calculateAllIn(QString,unsigned)));
	connect(&PokerClient::getInstance(), SIGNAL(newPublicCard()), this, SLOT(updatePublicCard()));
	connect(&PokerClient::getInstance(), SIGNAL(updatedUsersList(bool)), this, SLOT(update()));
	connect(&PokerClient::getInstance(), SIGNAL(activePlayer(QString)), this, SLOT(updateButtons(QString)));
	connect(&PokerClient::getInstance(), SIGNAL(newGame()), this, SLOT(prepareNewGame()));

    // setting up background
	QPalette *palette = new QPalette();
	palette->setBrush(QPalette::Background,*(new QBrush(*(new QPixmap(":/img/bg")))));
	setPalette(*palette);

    if(checkReady())
        PokerClient::getInstance().requestTableUnpause();
    else
        delete ui;
    prepareButtons();
	prepareSliderAndSpinBox(Model::getInstance().tableList[currentTable].small_blind,
						Model::getInstance().tableList[currentTable].buy_in, 0);

	update();
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::prepareButtons()
{
	ui->betRaiseButton->setText(QString("Raise"));
    ui->callCheckButton->setText(QString("Check"));
    ui->foldButton->setText(QString("Fold"));
    ui->allInButton->setText(QString("All in"));
	ui->betRaiseButton->setEnabled(false);
	ui->callCheckButton->setEnabled(false);
	ui->foldButton->setEnabled(false);
	ui->allInButton->setEnabled(false);

}

void GameWindow::prepareSliderAndSpinBox(unsigned step, unsigned max, unsigned min)
{
	ui->betSlider->setSingleStep((int)step);
	ui->betSlider->setRange((int)min, (int)max);
	ui->betSpinBox->setMinimum((int)min);
	ui->betSpinBox->setMaximum((int)max);
	ui->betSpinBox->setSingleStep((int)step);
	ui->betSlider->setValue((int)min);
}

bool GameWindow::checkReady()
{
    QMessageBox msg(QMessageBox::Information, QString("Buy in"), QString("Are you ready?"));
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Ok);
    return msg.exec();
}

void GameWindow::updateCash()
{
	std::list<QString>::iterator it = Model::getInstance().tableList[currentTable].playersQueue.begin();
	int i = 0;
	for(; it != Model::getInstance().tableList[currentTable].playersQueue.end(); ++it)
	{
		cash[i]->setText(QString::number(Model::getInstance().tableList[currentTable].players[(*it)].cash_available));
		++i;
	}
	ui->pot->setText(QString::number(pot));
}

void GameWindow::updatePlayers()
{
	std::list<QString>::iterator it = Model::getInstance().tableList[currentTable].playersQueue.begin();
	int i = 0;
	for(; it != Model::getInstance().tableList[currentTable].playersQueue.end(); ++it)
	{
		players[i]->setText((*it));
		QString src;
		Model::getInstance().tableList[currentTable].players[(*it)].seat = i;
		if((*it) == Model::getInstance().getUserName())
		{
			if(userSeat!=i && userSeat!=-1)
			{
				src = ":/cards/flipside";
				cards[userSeat][0]->setPixmap(*(new QPixmap(src)));
				cards[userSeat][1]->setPixmap(*(new QPixmap(src)));
				src = ":/cards/" + QString::number(Model::getInstance().cards[0]);
				cards[i][0]->setPixmap(*(new QPixmap(src)));
				src = ":/cards/" + QString::number(Model::getInstance().cards[1]);
				cards[i][1]->setPixmap(*(new QPixmap(src)));
			}
			userSeat = i;
		}
		else
		{
			QString src = ":/cards/flipside";
			cards[i][0]->setPixmap(*(new QPixmap(src)));
			cards[i][1]->setPixmap(*(new QPixmap(src)));
		}
		++i;
	}
}

void GameWindow::updateTableInfo()
{
	QStringList* list = new QStringList;
	std::list<QString>::iterator it = Model::getInstance().tableList[currentTable].playersQueue.begin();
	int i = 0;
	for(; it != Model::getInstance().tableList[currentTable].playersQueue.end(); ++it)
	{
		list->append((*it) + ' ' + QString::number(Model::getInstance().tableList[currentTable].players[(*it)].cash_available));
	}

	QStringListModel *model = new QStringListModel();
	model->setStringList(*list);

	ui->tableInfoList->setModel(model);
	ui->tableInfoList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void GameWindow::updateButtons(QString name)
{	

	if(name == Model::getInstance().getUserName())
	{
		ui->betRaiseButton->setEnabled(true);
		ui->callCheckButton->setEnabled(true);
		ui->foldButton->setEnabled(true);
		ui->allInButton->setEnabled(true);
	}
	else
	{
		ui->betRaiseButton->setEnabled(false);
		ui->callCheckButton->setEnabled(false);
		ui->foldButton->setEnabled(false);
		ui->allInButton->setEnabled(false);
	}
	ui->betRaiseButton->setText(QString("Raise"));
	ui->foldButton->setText(QString("Fold"));
	ui->allInButton->setText(QString("All in"));
	if(Model::getInstance().tableList[currentTable].last_action == Tableinfo::RAISED)  //there's been a bet, user can call
		ui->callCheckButton->setText(QString("Call"));
	else
		ui->callCheckButton->setText(QString("Check"));

	prepareSliderAndSpinBox(Model::getInstance().tableList[currentTable].small_blind,
							Model::getInstance().tableList[currentTable].players[Model::getInstance().getUserName()].cash_available,
							Model::getInstance().tableList[currentTable].last_bet);

	std::list<QString>::iterator it = Model::getInstance().tableList[currentTable].playersQueue.begin();
	int i = 0;
	for(; it != Model::getInstance().tableList[currentTable].playersQueue.end(); ++it)
	{
		if((*it) == name)
		{
			players[i]->setStyleSheet("QLabel{color: yellow; font-weight: bold;}");
		}
		else
		{
			players[i]->setStyleSheet("QLabel{color: white; font-weight: normal;}");
		}
		i++;
	}
}


void GameWindow::changeBetSpinBoxValue(int value)
{
	int temp;
	temp = value;
    if(temp < ui->betSlider->minimum())
            ui->betSpinBox->setValue(ui->betSlider->minimum());
    else
            ui->betSpinBox->setValue(temp);
}

void GameWindow::changeBetSliderValue(int value)
{
    ui->betSlider->setValue(value);
}

void GameWindow::playerBetRaise()
{
    int bet = ui->betSpinBox->value();
    Model::getInstance().setMoneyToRaise(bet);
    PokerClient::getInstance().requestTableRaise();
}

void GameWindow::playerCallCheck()
{
    PokerClient::getInstance().requestTableCheck();
}

void GameWindow::playerFold()
{
    PokerClient::getInstance().requestTableFold();
}

void GameWindow::playerAllIn()
{
    PokerClient::getInstance().requestTableAllIn();
}

void GameWindow::updatePrivateCard()
{
    int card = Model::getInstance().cards.size();
	QString src = ":/cards/" + QString::number(Model::getInstance().cards[card-1]);
    if(card == 1)
    {

		cards[userSeat][0]->setPixmap(*(new QPixmap(src)));
    }
    else
    {
		cards[userSeat][1]->setPixmap(*(new QPixmap(src)));
	}
}

void GameWindow::updatePrivateCard(QString who, unsigned card)
{
	static int whichCard = 1;
	if(whichCard == 1)
	{
		whichCard++;
	}
	else
	{
		QMessageBox msg(QMessageBox::Information, QString("Winner"), QString("Player " + who + " won " + QString::number(pot) + " cash!"));
		msg.setStandardButtons(QMessageBox::Ok);
		msg.setDefaultButton(QMessageBox::Ok);
		msg.exec();
		Model::getInstance().tableList[currentTable].players[who].cash_available += pot;
		pot = 0;
		whichCard = 1;
	}
}

void GameWindow::updatePublicCard()
{
	int card = Model::getInstance().tableList[currentTable].table_cards.size();
	QString src = ":/cards/" + QString::number(Model::getInstance().tableList[currentTable].table_cards[card-1]);
    if(card == 1)
    {
		ui->card1->setPixmap(*(new QPixmap(src)));
    }
    else if(card == 2)
    {
		ui->card2->setPixmap(*(new QPixmap(src)));
    }
    else if(card == 3)
	{
		ui->card3->setPixmap(*(new QPixmap(src)));
    }
    else if(card == 4)
    {
		ui->card4->setPixmap(*(new QPixmap(src)));
    }
    else if(card == 5)
    {
		ui->card5->setPixmap(*(new QPixmap(src)));
	}
}

void GameWindow::update()
{
	updatePlayers();
	updateCash();
	updateTableInfo();
}

void GameWindow::prepareNewGame()
{
	PokerClient::getInstance().requestUsersList(currentTable);
	ui->card1->setPixmap(*new QPixmap());
	ui->card2->setPixmap(*new QPixmap());
	ui->card3->setPixmap(*new QPixmap());
	ui->card4->setPixmap(*new QPixmap());
	ui->card5->setPixmap(*new QPixmap());
	Model::getInstance().cards.clear();
	Model::getInstance().tableList[currentTable].table_cards.clear();
	ui->pot->setText(QString::number(pot));
}

void GameWindow::calculateRaise(QString name, unsigned money)
{
	Model::getInstance().tableList[currentTable].players[name].cash_available -= money;
	pot+=money;
	update();
}

void GameWindow::calculateAllIn(QString name, unsigned money)
{
	int allin = Model::getInstance().tableList[currentTable].players[name].cash_available;
	Model::getInstance().tableList[currentTable].players[name].cash_available = 0;
	pot += allin;
	update();
}

void GameWindow::createPointers()
{
	players[0] = ui->playerName_1;
	players[1] = ui->playerName_2;
	players[2] = ui->playerName_3;
	players[3] = ui->playerName_4;
	players[4] = ui->playerName_5;
	players[5] = ui->playerName_6;
	players[6] = ui->playerName_7;
	players[7] = ui->playerName_8;

	cash[0] = ui->playerCash_1;
	cash[1] = ui->playerCash_2;
	cash[2] = ui->playerCash_3;
	cash[3] = ui->playerCash_4;
	cash[4] = ui->playerCash_5;
	cash[5] = ui->playerCash_6;
	cash[6] = ui->playerCash_7;
	cash[7] = ui->playerCash_8;

	cards[0][0] = ui->card1_1;
	cards[0][1] = ui->card1_2;
	cards[1][0] = ui->card2_1;
	cards[1][1] = ui->card2_2;
	cards[2][0] = ui->card3_1;
	cards[2][1] = ui->card3_2;
	cards[3][0] = ui->card4_1;
	cards[3][1] = ui->card4_2;
	cards[4][0] = ui->card5_1;
	cards[4][1] = ui->card5_2;
	cards[5][0] = ui->card6_1;
	cards[5][1] = ui->card6_2;
	cards[6][0] = ui->card7_1;
	cards[6][1] = ui->card7_2;
	cards[7][0] = ui->card8_1;
	cards[7][1] = ui->card8_2;
}

void GameWindow::closeEvent(QCloseEvent *event)
{
	PokerClient::getInstance().requestTableLeave();
	PokerClient::getInstance().requestAllUsers();
}
