#include <QtOpenGL>

#include "SmoothShadedMode.h"

SmoothShadedMode::SmoothShadedMode() :
	ShadedMode(true, false, true)
{
	// Nothing to do...
}

QString SmoothShadedMode::name() const
{
	return tr("Smooth Shaded Mode");
}
