/*
 * mainwindow.h
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#ifndef DIAGNOSTIC_MAINWINDOW_H_
#define DIAGNOSTIC_MAINWINDOW_H_

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	Ui::MainWindow *ui;

	QTcpSocket* socket = nullptr;

	void read_data ();

	Q_SLOT void on_connect_clicked ();

	static void show_message (const std::string& msg);

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
};


#endif /* DIAGNOSTIC_MAINWINDOW_H_ */
