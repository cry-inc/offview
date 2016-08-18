#include <QtCore>

#include "SceneFactory.h"
#include "OffScene.h"

QString SceneFactory::openFileString()
{
	// Seperate additional file formats with ';;'
	// Example: "Off Files (*.off);;Bla files (*.bla)"
	return QString(tr("Off Files (*.off)"));
}

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
