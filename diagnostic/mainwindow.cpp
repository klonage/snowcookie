/*
 * mainwindow.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "protocol/edison_frame.h"
#include "protocol/protocol_utils.h"

#include <QPushButton>
#include <QMessageBox>

using namespace SnowCookie;

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QObject::connect (ui->connectButton, &QPushButton::clicked, this, &MainWindow::on_connect_clicked);

	QObject::connect (ui->reloadStatusButton, &QPushButton::clicked, [this] {
		GetStatusEdisonFrame frame;
		unsigned char data [GetStatusEdisonFrame::max_size], dest [GetStatusEdisonFrame::max_size * 2 + 1];
		int size = frame.serialize (data);
		size = pack_frame (data, size, dest, substitute_character, xor_character, end_character);
		QByteArray arr ((char*) dest, size);
		socket->write (arr);
	});
}

MainWindow::~MainWindow()
{
	delete ui;
	delete socket;
}

void MainWindow::on_connect_clicked()
{
	socket = new QTcpSocket (this);
	connect (socket, SIGNAL (readyRead ()), SLOT (read_data ()));

	socket->connectToHost (ui->ipLineEdit->text (), ui->portLineEdit->text ().toInt ());
	if(socket->waitForConnected (3000))
		show_message ("connected!");
	else
	{
		show_message ("connection timeout");
	}
}

void MainWindow::read_data ()
{
	QByteArray data = socket->readAll();
}

void MainWindow::show_message (const std::string& msg)
{
	QMessageBox msgBox;
	msgBox.setText (QString::fromStdString (msg));
	msgBox.exec();
}
