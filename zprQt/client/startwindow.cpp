#include "startwindow.h"
#include "ui_startwindow.h"
#include "pokerclient.h"
#include "model.h"
#include "lobbygui.h"
#include "ui_lobbygui.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    isConnected = false;
    PokerClient::getInstance();
    Model::getInstance();
	ui->connectButton->setDefault(true);
    connect(&PokerClient::getInstance(), SIGNAL(loggedIn(bool)), this, SLOT(loginCheck(bool)));
	connect(ui->password, SIGNAL(returnPressed()), this, SLOT(on_connectButton_clicked()));
	connect(&PokerClient::getInstance(), SIGNAL(connected(bool)), this, SLOT(checkPassword(bool)));
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::loginCheck(bool check)
{
    if(check)
        startTheApp();
    else
		clean("Login check failed");
}

void StartWindow::clean(QString message)
{
	ui->info->setText(message);
    ui->login->setText("");
    ui->password->setText("");
}

void StartWindow::startTheApp()
{
    ui->info->setText("Connected");
    Lobby *w = new Lobby();
    w->show();
	this->hide();
}

void StartWindow::on_connectButton_clicked()
{
    Model::getInstance().setHostAddress(ui->serverip->text());
    Model::getInstance().setUserName(ui->login->text());
    Model::getInstance().setUserPass(ui->password->text());
	PokerClient::getInstance().connectToServer();
}

void StartWindow::on_cancelButton_clicked()
{
	QApplication::quit();
}

void StartWindow::checkPassword(bool b)
{
	if(b)
		PokerClient::getInstance().checkPassword();
	else
		clean("Connection error");
}
