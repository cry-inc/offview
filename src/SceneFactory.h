#pragma once

#include "IScene.h"

/**
 * @brief Class for format independent file loading
 *
 * This class is used to seperate the model loading code from the
 * other classes, so it is easy to add a new file format/parser.
 *
 * @author M. Caputo
 * @date 2010-04-20
 */
class SceneFactory
{
	Q_DECLARE_TR_FUNCTIONS(SceneFactory)

public:

	/**
	 * @brief Getter for the filter string
	 *
	 * This static method is used to supply the file type string for
	 * the open file dialog.
	 *
	 * @return Returns the filter string for QFileDialog
	 */
	static QString openFileString();

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
	static IScene* openFile(QString file);
};
