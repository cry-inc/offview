/**
 * @file FlatShadedMode.cpp
 * @see FlatShadedMode
 * @see IRenderMode
 * @author M. Caputo
 * @date 2010-03-26
 */

#include <QtOpenGL>
#include "FlatShadedMode.h"

/**
 * @brief Constructor
 *
 * Does nothing but call the base contructor of ShadedMode with the
 * right settings. All the work will be done by the base class!
 */
FlatShadedMode::FlatShadedMode() : ShadedMode(false, false, true)
{}

QString FlatShadedMode::name() const
{
	return tr("Flat Shaded Mode");
}
