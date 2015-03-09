/*
 * mainwindow.h
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#ifndef DIAGNOSTIC_MAINWINDOW_H_
#define DIAGNOSTIC_MAINWINDOW_H_

#include "protocol/data_parser.h"
#include "protocol/edison_frame.h"

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	SnowCookie::DataParser parser;

	// todo multidefinition
	const unsigned char end_character = 0x0A;
	const unsigned char substitute_character = 0x1A;
	const unsigned char xor_character = 0x33;

	Ui::MainWindow *ui;

	QTcpSocket* socket = nullptr;

	Q_SLOT void read_data ();

	void handle_buffer(SnowCookie::DataBuffer buffer);

	void update_status (std::shared_ptr<SnowCookie::GetStatusEdisonFrame> frame);

	Q_SLOT void on_connect_clicked ();

	static void show_message (const std::string& msg);

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
};


#endif /* DIAGNOSTIC_MAINWINDOW_H_ */
