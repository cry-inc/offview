/**
 * @class ColoredMode
 * @brief Colored render mode
 *
 * A render mode for colored scenes. Supports colored vertices
 * and colored polygons. This mode mode is based on ShadedMode.
 *
 * @see ShadedMode
 * @see IRenderMode
 * @author M. Caputo
 * @date 2010-04-20
 */

#ifndef COLORED_MODE_H
#define COLORED_MODE_H

#include "ShadedMode.h"

class ColoredMode : public ShadedMode
{
	Q_DECLARE_TR_FUNCTIONS(ColoredMode)

public:

	ColoredMode();
	QString name() const override;

};

#endif
