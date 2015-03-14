/**
 * @class SceneFactory
 * @brief Class for format independent file loading
 *
 * This class is used to seperate the model loading code from the
 * other classes, so it is easy to add a new file format/parser.
 *
 * @author M. Caputo
 * @date 2010-04-20
 */

#ifndef SCENE_FACTORY_H
#define SCENE_FACTORY_H

#include "IScene.h"

class SceneFactory
{

	Q_DECLARE_TR_FUNCTIONS(SceneFactory)

public:

	static QString openFileString();
	static IScene* openFile(QString file);
};

#endif
