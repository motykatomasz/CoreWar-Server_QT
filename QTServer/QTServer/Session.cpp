#include "Session.h"




Session::Session(int c, int t, QString w1, QString def, QTcpSocket* s1, QString n)
{
	core = c;
	turns = t;
	warrior_1 = w1;
	defaultInstruction = def;
	client_1 = s1;
	name = n;
	isFull = false;

}


Session::~Session()
{


}

void Session::addClient2(QString w2, QTcpSocket* s2)
{
	warrior_2 = w2;
	client_2 = s2;
	isFull = true;

	sendBoard();

}

void Session::sendBoard()
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);

	out << (qint16)3;

	out << (qint16)core;
	out << warrior_1;
	out << warrior_2;
	out << defaultInstruction;

	sendMesage(block);

}


void Session::actualizeBoard(int address, QString inst)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);

	out << (qint16)4;	
	out << (qint16)address;
	out << inst;

	sendMesage(block);
}

void Session::sendMesage(QByteArray &message)
{

	client_1->write(message);
	client_2->write(message);

}
