/**
 * @class ShadedMode
 * @brief Base class for shaded render modes
 *
 * This abstract class provides methods for smooth and flat shaded objects.
 * The brightness of each surface point depends on its angle torwards the
 * light source. To use the flat shaded render mode, all polygons must
 * provide normal vectors. To use the smooth shaded render mode, all vertices
 * must provide normal vectors.
 *
 * @see IRenderMode
 * @author M. Caputo
 * @date 2010-06-11
 */

#ifndef SHADED_MODE_H
#define SHADED_MODE_H

#include "IRenderMode.h"

class ShadedMode : public IRenderMode
{

public:

	ShadedMode(bool smoothShaded, bool colored, bool specular);
	void setSettings();
	void unsetSettings();
	void draw(const IScene *scene, const QColor *color);
	
private:

	void createDisplayList(const IScene *scene, const QColor *color);

	/**
	 * @brief Stores the display list identifier
	 */
	unsigned int displayList;

	/**
	 * @brief Used to determine whether the scene has changed
	 */
	const IScene *oldScene;

	/**
	 * @brief Used to determine whether the color has changed
	 */
	QColor oldColor;
	
	/**
	 * @brief Smooth shading or flat shading?
	 */
	bool smoothShaded;
	
	/**
	 * @brief Colored or uncolored mode?
	 */
	bool colored;
	
	/**
	 * @brief Specular light effects enabled?
	 */
	bool specular;

};

#endif
