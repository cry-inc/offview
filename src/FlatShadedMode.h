/**
 * @class FlatShadedMode
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

#ifndef FLATSHADED_MODE_H
#define FLATSHADED_MODE_H

#include "ShadedMode.h"

class FlatShadedMode : public ShadedMode
{
	Q_DECLARE_TR_FUNCTIONS(FlatShadedMode)

public:

	FlatShadedMode();
	QString name() const override;

};

#endif
