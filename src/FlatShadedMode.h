#pragma once

#include "ShadedMode.h"

/**
 * @brief Flat shaded render mode
 *
 * This mode displays the object as a solid and flat shaded object.
 * The brightness of each polygon depends on its angle torwards
 * the light source. The normal vectors for all polygons are needed!
 *
 * @see IRenderMode
 * @see ShadedMode
 * @author M. Caputo
 * @date 2010-03-26
 */
class FlatShadedMode : public ShadedMode
{
	Q_DECLARE_TR_FUNCTIONS(FlatShadedMode)

public:
	/**
	 * @brief Constructor
	 *
	 * Does nothing but call the base contructor of ShadedMode with the
	 * right settings. All the work will be done by the base class!
	 */
	FlatShadedMode();

	QString name() const override;
};
