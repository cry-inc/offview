/**
 * @file SceneFactory.cpp
 * @see SceneFactory
 * @author M. Caputo
 * @date 2010-04-20
 */

#include <QtCore>

#include "SceneFactory.h"
#include "OffScene.h"

/**
 * @brief Getter for the filter string
 *
 * This static method is used to supply the file type string for
 * the open file dialog.
 *
 * @return Returns the filter string for QFileDialog
 */
QString SceneFactory::openFileString()
{
	// Seperate additional file formats with ';;'
	// Example: "Off Files (*.off);;Bla files (*.bla)"
	return QString(tr("Off Files (*.off)"));
}

/**
 * @brief Loads a file into memory
 *
 * This static method takes a path to an model file and determines
 * with the help of the file extension which parser to use.
 * The parser class of each format must implement the IScene interface,
 * or else we are not able to return a reference to a scene object!
 * This method will throw a string if somethings goes wrong!
 *
 * @param [in] file Path to the file which should be parsed
 * @return Returns a pointer to an scene object, see IScene
 */
IScene* SceneFactory::openFile(QString file)
{
	QFileInfo fileInfo(file);
	
	if (!fileInfo.exists()) {
		throw QString(tr("File does not exist!"));
	}
	
	QString ext = fileInfo.suffix();
	if (ext == "off") {
		return new OffScene(file);
	} else {
		throw QString(tr("File format not supported!"));
	}
}
