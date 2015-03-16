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

	ui->framesTreeWidget->setHeaderLabels({"frame type", "frame size", "frame data"});

	QObject::connect (ui->connectButton, &QPushButton::clicked, this, &MainWindow::on_connect_clicked);

	QObject::connect (ui->stopDisablingWifiButton, &QPushButton::clicked, this, &MainWindow::on_stopDisablingWifi_clicked);

	QObject::connect (ui->sendRawButton, &QPushButton::clicked, this, &MainWindow::on_sendRaw_clicked);

	QObject::connect (ui->clearTreeButton, &QPushButton::clicked, [this] {
		ui->framesTreeWidget->clear();
	});

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

void MainWindow::on_sendRaw_clicked ()
{
	auto str_bytes = ui->rawDataEdit->text ().split (' ');
	char* bytes = new char[str_bytes.length ()];
	char* dest_bytes = nullptr;
	bool ok;
	int base = ui->hexRawRadio->isChecked () ? 16 : 10;

	try
	{
		for (int i = 0; i < str_bytes.length (); i++)
		{
			bytes[i] = str_bytes [i].toInt (&ok, base);
			if (!ok)
				throw std::runtime_error ("cannot parse numbers");
		}

		dest_bytes = new char[str_bytes.length ()*2];
		int new_size = pack_frame (bytes, str_bytes.length (), dest_bytes);
		socket->write (dest_bytes, new_size);
	}
	catch (const std::exception& ex)
	{
		show_message (std::string ("Exception: ") + ex.what ());
	}
	delete bytes;
	delete dest_bytes;

	if (ui->clearAfterSendCheckBox->isChecked ())
		ui->rawDataEdit->setText ("");
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
	std::shared_ptr<EdisonFrame> frame;
	bool err = false;
	try
	{
		frame = EdisonFrame::parse_frame (buffer.frame, buffer.size);

		switch (frame->get_type ())
		{
		case EdisonFrame::GET_STATUS:
			update_status (std::static_pointer_cast<GetStatusEdisonFrame>(frame));
			break;
		default:
			break;
		}
	}
	catch (...)
	{
		err = true;
	}

	QString res_hex;
	QByteArray ba (buffer.frame, buffer.frame_size);
	for (int i = 0; i < ba.size(); i++)
		res_hex.append ("0x").append (QString::number(ba.at(i), 16).rightJustified(2, '0')).append (' ');

	ui->framesTreeWidget->insertTopLevelItem(0, new QTreeWidgetItem({
		QString::fromStdString(err ? std::string("unknown frame") : std::to_string(frame->get_type ())),
				QString::fromStdString(std::to_string(buffer.frame_size)), res_hex}));
}

void MainWindow::update_status (std::shared_ptr<SnowCookie::GetStatusEdisonFrame> frame)
{
	ui->diskUsageLabel->setText (QString::number (frame->get_size ()/1024/1024.0, 'f', 3));
	ui->logFilesCountLabel->setText (QString::number (frame->get_log_count ()));
}

void MainWindow::on_stopDisablingWifi_clicked ()
{
	char dest [WifiEdisonFrame::max_size * 2 + 1];
	int size = EdisonFrame::pack_and_serialize(WifiEdisonFrame (), dest);
	QByteArray arr ((char*) dest, size);
	socket->write (arr);
}
