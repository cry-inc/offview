/**
 * @file SmoothShadedMode.cpp
 * @see SmoothShadedMode
 * @see IRenderMode
 * @author M. Caputo
 * @date 2010-06-11
 */

#include <QtOpenGL>
#include "SmoothShadedMode.h"

/**
 * @brief Constructor
 *
 * Does nothing but call the base contructor of ShadedMode with the
 * right settings. All the work will be done by the base class!
 */
SmoothShadedMode::SmoothShadedMode() : ShadedMode(true, false, true)
{}

QString SmoothShadedMode::name()
{
	return tr("Smooth Shaded Mode");
}
