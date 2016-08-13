#include <QtOpenGL>
#include "ColoredMode.h"

ColoredMode::ColoredMode() :
	ShadedMode(/* smoothShaded = */ true, /* colored = */ true, /* specular = */ true)
{
	// Nothing to do
}

QString ColoredMode::name() const
{
	return tr("Colored Mode");
}
