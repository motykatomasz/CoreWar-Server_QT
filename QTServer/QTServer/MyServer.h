#pragma once

#ifndef MYSERVER_H
#define MYSERVER_H
#include <QTcpServer>
#include "Session.h"
#include <qdatastream.h>

class MyServer : public QTcpServer
{
	Q_OBJECT
public:
	MyServer(QObject *parent = 0);
	void sendToAllConnections(QByteArray& message);
	void startServer();

	private slots:
	/**
	* @brief Metoda zapisuj¹ca przychodz¹ce po³¹czenie od klienta
	*/
	void slotNewConnection();

	/**
	* @brief Metoda zczytuj¹ca dane jeœli pojawi¹ siê od któregoœ z klientów
	*/
	void readyRead();

private:
	QList<QTcpSocket*> clientConnections;	//lista wszystkich graczy pod³¹czonych do serwera
	QMap<QString, Session*> sessions;		//s³ownik aktualnie trwaj¹cych sesji gry

	/**
	* @brief Metoda tworz¹ca now¹ sesjê gry
	*/
	void createSession(QTcpSocket* sock, int core, int turn, QString ins, QString warrior1, QString name);

	/**
	* @brief Metoda dodaj¹ca drugiego gracza do sesji i rozpoczynaj¹ca grê
	*/
	void addPlayer(QTcpSocket* sock, QString warrior2, QString name);
};




#endif // MYSERVER_H