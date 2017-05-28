#include "QTServer.h"
#include <QtWidgets/QApplication>
#include "myserver.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QTServer w;
	//w.show();
	MyServer server;
	server.startServer();
	return a.exec();
}
