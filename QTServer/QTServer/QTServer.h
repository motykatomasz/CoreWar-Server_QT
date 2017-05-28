#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QTServer.h"

class QTServer : public QMainWindow
{
	Q_OBJECT

public:
	QTServer(QWidget *parent = Q_NULLPTR);

private:
	Ui::QTServerClass ui;
};
