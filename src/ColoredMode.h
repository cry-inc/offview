#pragma once

#include "ShadedMode.h"

/**
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
class ColoredMode : public ShadedMode
{
	Q_DECLARE_TR_FUNCTIONS(ColoredMode)

public:
	/**
	 * @brief Constructor
	 *
	 * Does nothing but call the base contructor of ShadedMode with the
	 * right settings. All the work will be done by the base class!
	 */
	ColoredMode();

	QString name() const override;
};
