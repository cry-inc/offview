/**
* @file main.cpp
*
* @author Manuel Caputo
* @author Markus Haecker
* @date 2010-05-26
*
* @mainpage OffView
*
* This is the source code documentation for our project OffView generated 
* with Doxygen.
*
* You can find the OffView project page at Google Code:
* <a href="http://code.google.com/p/offview">http://code.google.com/p/offview</a>.
*/

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "MainWindow.h"

/**
* @brief Creates our QApplication object and its main window.
*
* Before creating our main window, we check first if the current system has
* OpenGL support; if not, terminate the application with an error message.
* 
* @param[in] argc Number of arguments
* @param[in] argv Argument vector
*/
int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	
	// Check for OpenGL support
	if(!QGLFormat::hasOpenGL()) {
		QMessageBox::critical(
			0, QApplication::tr("Warning"),
			QApplication::tr("This system has no OpenGL support!")
		);
		return -1;
	}

	// Check for a supplied file from the arguments
	QString file = "";
	if (argc > 1) {
		file = argv[1];
	}
	
	// Set up main window and start the application
	MainWindow window(file);
	window.show();
	return app.exec();
}
