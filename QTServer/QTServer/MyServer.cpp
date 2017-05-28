#include "MyServer.h"

#include <qdatastream.h>


MyServer::MyServer(QObject *parent)
	: QTcpServer(parent)
{
	connect(this, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

void MyServer::sendToAllConnections(QByteArray &message)
{
	for (int i = 0; i < clientConnections.size(); i++)
	{
		clientConnections[i]->write(message);
	}
}

void MyServer::startServer()
{
	int port = 9999;

	if (!this->listen(QHostAddress::Any, port))
	{
		qDebug() << "Could not start server";
	}
	else
	{
		qDebug() << "Listening to port " << port << "...";
	}
}

void MyServer::readyRead()
{
	QTcpSocket* socket = qobject_cast<QTcpSocket*>(QObject::sender());
	QDataStream in(socket);
	quint16 msg_type;
	in.setVersion(QDataStream::Qt_4_0);
	
	in >> msg_type;
	
	qDebug() << " Data in: " << msg_type;

	//tworzenie nowej sesji
	if (msg_type == 1)
	{
		qint16 core;
		qint16 turns;
		QString name;
		QString ins;
		QString warrior;
		
		in >> core;
		in >> turns;
		in >> ins;
		in >> warrior;
		in >> name;
		qDebug() << "Core: " << core;
		qDebug() << "Turns: " << turns;
		qDebug() << "Ins: " << ins;

		createSession(socket, core, turns, ins, warrior, name);
	}

	//do³¹czanie do istniej¹cej sesji
	else if (msg_type == 2) 
	{
		QString name;
		QString warrior;

		in >> name;
		in >> warrior;

		addPlayer(socket, warrior, name);
	
	}

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);

	out << (qint16)2;
	
}

void MyServer::createSession(QTcpSocket * sock, int core, int turn, QString ins, QString warrior1, QString name)
{

	QMap<QString, Session*>::const_iterator i = sessions.find(name);
	if (i == sessions.end()) 
	{
		Session* session = new Session(core, turn, warrior1, ins, sock, name);
		this->sessions.insert(name, session);
	}

	else
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);

		QString err = "Session with this name already exists!!! Please specify other name.";

		out << (qint16)6;
		out << err;

		sock->write(block);
	}
}

void MyServer::addPlayer(QTcpSocket *sock, QString warrior2, QString name)
{
	QMap<QString, Session*>::const_iterator i = sessions.find(name);

	if(!this->sessions[name]->isFull)
		this->sessions[name]->addClient2(warrior2, sock);

	else if (i == sessions.end())
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);

		QString err = "Session with specified name doesn't exist.";

		out << (qint16)6;
		out << err;

		sock->write(block);
	}
	else 
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);

		QString err = "Session is full. Please find another session or create your own.";

		out << (qint16)6;
		out << err;

		sock->write(block);
	
	}
}

void MyServer::slotNewConnection()
{
	QTcpSocket *clientConnection = new QTcpSocket();;

	while (hasPendingConnections()) {
		clientConnection = nextPendingConnection();

		clientConnections.append(clientConnection);
		connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readyRead()));
	}
}

