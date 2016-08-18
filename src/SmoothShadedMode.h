#pragma once

#include "ShadedMode.h"

/**
 * @brief Smooth shaded render mode
 *
 * This mode displays the object as a solid and smooth shaded object.
 * The brightness of each surface point depends on its angle torwards
 * the light source. The normal vectors of all vertices are needed for this!
 *
 * @see IRenderMode
 * @see ShadedMode
 * @author M. Caputo
 * @date 2010-06-11
 */
class SmoothShadedMode : public ShadedMode
{
	Q_DECLARE_TR_FUNCTIONS(SmoothShadedMode)

public:
	/**
	 * @brief Constructor
	 *
	 * Does nothing but call the base contructor of ShadedMode with the
	 * right settings. All the work will be done by the base class!
	 */
	SmoothShadedMode();

	QString name() const override;
};
