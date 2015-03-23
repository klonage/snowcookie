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
		char data [GetStatusEdisonFrame::max_size], dest [GetStatusEdisonFrame::max_size * 2 + 1];
		int size = frame.serialize (data);
		size = pack_frame (data, size, dest);
		QByteArray arr ((char*) dest, size);
		socket->write (arr);
	});

	QObject::connect (ui->startLogPushButton, &QPushButton::clicked, [this] {
		send_simple_log (EdisonFrame::START_LOG);
	});
	QObject::connect (ui->stopLogPushButton, &QPushButton::clicked, [this] {
		send_simple_log (EdisonFrame::STOP_LOG);
	});
	QObject::connect (ui->clearLogPushButton, &QPushButton::clicked, [this] {
		send_simple_log (EdisonFrame::CLEAR_LOGS);
	});
	QObject::connect (ui->setDivisorPushButton, &QPushButton::clicked, [this] {
		DivisorEdisonFrame frame (
				(char)ui->destLineEdit->text().toInt(),
				ui->locationLineEdit->text().toInt(),
				ui->divisorLineEdit->text().toInt()
				);
		char dest [DivisorEdisonFrame::max_size * 2 + 1];
		int size = EdisonFrame::pack_and_serialize(frame, dest);
		QByteArray arr ((char*) dest, size);
		socket->write (arr);
	});
}

void MainWindow::send_simple_log (EdisonFrame::Type t)
{
	SimpleLogEdisonFrame frame (t);
	char dest [SimpleLogEdisonFrame::max_size * 2 + 1];
	int size = frame.pack_and_serialize(frame, dest);
	QByteArray arr ((char*) dest, size);
	socket->write (arr);
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

	parser.register_handler ([this] (DataBuffer buf) {
		handle_buffer (buf);
	});

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

	parser.append_bytes (data.constData (), data.size ());
}

void MainWindow::show_message (const std::string& msg)
{
	QMessageBox msgBox;
	msgBox.setText (QString::fromStdString (msg));
	msgBox.exec();
}

void MainWindow::handle_buffer(SnowCookie::DataBuffer buffer)
{
	auto frame = EdisonFrame::parse_frame (buffer.frame, buffer.size);

	switch (frame->get_type ())
	{
	case EdisonFrame::GET_STATUS:
		update_status (std::static_pointer_cast<GetStatusEdisonFrame>(frame));
		break;
	default:
		break;
	}
}

void MainWindow::update_status (std::shared_ptr<SnowCookie::GetStatusEdisonFrame> frame)
{
	ui->diskUsageLabel->setText (QString::number (frame->get_size ()/1024/1024.0, 'f', 3));
	ui->logFilesCountLabel->setText (QString::number (frame->get_log_count ()));
}
