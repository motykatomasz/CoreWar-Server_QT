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
	* @brief Metoda zapisuj�ca przychodz�ce po��czenie od klienta
	*/
	void slotNewConnection();

	/**
	* @brief Metoda zczytuj�ca dane je�li pojawi� si� od kt�rego� z klient�w
	*/
	void readyRead();

private:
	QList<QTcpSocket*> clientConnections;	//lista wszystkich graczy pod��czonych do serwera
	QMap<QString, Session*> sessions;		//s�ownik aktualnie trwaj�cych sesji gry

	/**
	* @brief Metoda tworz�ca now� sesj� gry
	*/
	void createSession(QTcpSocket* sock, int core, int turn, QString ins, QString warrior1, QString name);

	/**
	* @brief Metoda dodaj�ca drugiego gracza do sesji i rozpoczynaj�ca gr�
	*/
	void addPlayer(QTcpSocket* sock, QString warrior2, QString name);
};




#endif // MYSERVER_H