#include <QtOpenGL>
#include "FlatShadedMode.h"

FlatShadedMode::FlatShadedMode() :
	ShadedMode(/* smoothShaded = */ false, /* colored = */ false, /* specular = */ true)
{
	// Nothing to do
}

QString FlatShadedMode::name() const
{
	return tr("Flat Shaded Mode");
}
