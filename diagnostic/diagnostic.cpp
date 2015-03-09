/*
 * diagnostic.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: loganek
 */

#include "mainwindow.h"
#include "edison-board/logger.h" // todo move to another project

#include <QApplication>

int main (int argc, char **argv)
{
	QApplication app (argc, argv);

	MainWindow window;
	window.show ();

	return app.exec ();
}
