/*
====================================================================================

File: main.cpp
Description: This is the function from where the program is launched.
Copyright (C) 2010  OpenPIV (http://www.openpiv.net)

Contributors to this code:
Zachary Taylor

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

====================================================================================
*/

#ifndef _GTEST_
#include <QtGui/QApplication>
#include "./src/mainwindow.h"
//#include "QWT/curvdemo2.cpp"
//#include "QWT/spectrogram.cpp"
#else
#include <stdlib.h>
#endif


#ifndef _GTEST_
int main(int argc, char *argv[])
{
	//curvdemo2( argc, argv );
	//spectrogram( argc, argv );
	

	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
#else
int	main( int argc, char* argv[] )
{
	system("PAUSE");
	return	0;
}
#endif
