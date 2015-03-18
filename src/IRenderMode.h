/**
 * @class IRenderMode
 * @brief The abstract interface for all render modes
 *
 * Provides an abstract interface for custom OpenGL rendering modes.
 *
 * @author M. Caputo
 * @date 2010-03-26
 */

#ifndef I_RENDER_MODE_H
#define I_RENDER_MODE_H

#include <QString>

#include "IScene.h"

class IRenderMode
{

public:
	virtual ~IRenderMode() {}

	/**
	 * @brief Getter for the render mode name
	 *
	 * Name of the render mode. Should be enclosed by tr() for translation.
	 *
	 * @return The name of the render mode
	 */
	virtual QString name() const = 0;
	
	/**
	 * @brief Render mode startup code
	 *
	 * Will be called once when the render mode is changed.
	 */
	virtual void setSettings() = 0;
	
	/**
	 * @brief Render mode shutdown code
	 *
	 * When a render mode is deactivated, this method will be called,
	 * it should disable and undo all things from setSettings().
	 */
	virtual void unsetSettings() = 0;
	
	/**
	 * @brief Draws the scene
	 *
	 * Is called by the GlWIdget every time the scene needs to be redrawn.
	 * The grid and coordinate axes are already drawn at this point.
	 * 
	 * @param scene			The scene which should be displayed
	 * @param defaultColor	The default object color, only needed if the scene 
	 * 						itself is uncolored!
	 */
	virtual void draw(const IScene *scene, const QColor *defaultColor) = 0;
};

#endif
