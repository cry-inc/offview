/**
 * @file ColoredMode.cpp
 * @see ColoredMode
 * @see IRenderMode
 * @author M. Caputo
 * @date 2010-04-20
 */

#include <QtOpenGL>
#include "ColoredMode.h"

/**
 * @brief Constructor
 *
 * Does nothing but call the base contructor of ShadedMode with the
 * right settings. All the work will be done by the base class!
 */
ColoredMode::ColoredMode() : ShadedMode(true, true, true)
{}

QString ColoredMode::name() const
{
	return tr("Colored Mode");
}
