#pragma once

#include <QMainWindow>

namespace Ui {
    class StartWindow;
}
/**
 * Klasa odpowiadajaca za wyswietlanie okna logowania
 */
class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
	/**
	 * Konstruktor klasy
	 */
    explicit StartWindow(QWidget *parent = 0);
	/**
	 * Destruktor klasy
	 */
	~StartWindow();


private slots:
    void loginCheck(bool);
    void clean(QString message);
	void startTheApp();
    void on_connectButton_clicked();
	void checkPassword(bool b);
	void on_cancelButton_clicked();

private:
    Ui::StartWindow *ui;
    bool isConnected;
};
