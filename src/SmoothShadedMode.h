/**
 * @class SmoothShadedMode
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

#ifndef SMOOTHSHADED_MODE_H
#define SMOOTHSHADED_MODE_H

#include "ShadedMode.h"

class SmoothShadedMode : public ShadedMode
{
	Q_DECLARE_TR_FUNCTIONS(SmoothShadedMode)

public:

	SmoothShadedMode();
	QString name();

};

#endif
