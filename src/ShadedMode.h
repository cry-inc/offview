#pragma once

#include "IRenderMode.h"

/**
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
class ShadedMode : public IRenderMode
{
public:
	/**
	 * @brief Constructor
	 *
	 * Constructor which can create eight different render modes:
	 * Uncolored flat shading, uncolored smooth shading, colored flat shading,
	 * colored smooth shading and all of these modes with or without specular
	 * highlights.
	 *
	 * @param [in] smoothShaded Enables smooth shading. False means flat shading.
	 * @param [in] colored Enables colored mode. False means uncolored mode.
	 * @param [in] specular Enables specular highlights. False means no specular effects.
	 */
	ShadedMode(bool smoothShaded, bool colored, bool specular);

	void setSettings() override;
	
	void unsetSettings() override;

	/**
	 * @brief Draws a flat or smooth shaded IScene object
	 *
	 * This is an implenentation of IRenderMode::draw() with an additional
	 * parameter to control the shading. To use it, you can
	 * inherit a new render mode from this class and delegate draw() to
	 * this method!
	 *
	 * @param [in] scene The scene which should be drawn
	 * @param [in] defaultColor The default scene color
	 */
	void draw(const IScene *scene, const QColor *defaultColor) override;
	
private:
	/**
	 * @brief Creates a display list for the object 
	 *
	 * To get more perfomance out of OpenGL we use the display list
	 * feature. This can create a precompiled list of OpenGL commands
	 * which can be drawn much faster than with the normal immediate mode.
	 * The refenece number for the display list will be stored in the
	 * object variable displayList.
	 *
	 * @see ShadedMode::displayList
	 * @param [in] scene The scene which should be drawn
	 * @param [in] color Default model color
	 */
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
