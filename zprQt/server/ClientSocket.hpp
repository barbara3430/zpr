#pragma once

#include <QTcpSocket>
#include <boost/shared_ptr.hpp>

class Message;

class ClientSocket : public QObject
{
	Q_OBJECT
public:
	ClientSocket(int socketDescriptor, QObject* = 0);
	~ClientSocket();

	bool isActive();
	void emmiter();
signals:
	/**
	 * sygnal wysylany po zamknieciu socket'u
	 */
	void connectionClosed();
private slots:
	/**
	 * czytanie danych z socket'u
	 */
	void readClient();
	/**
	 * informacja o zerwaniu polaczenia przez klienta
	 */
	void discardClient();
public slots:
	/**
	 * odeslanie odpowiedzi do klienta
	 */
	void sendResponse(boost::shared_ptr<Message>);
private:
	/**
	 * rozlaczenie danego polaczenia
	 */
	void disconnect();
	quint16 nextBlockSize;
	QTcpSocket* socket;
};
