/**
 * @class WireframeMode
 * @brief Wireframe render mode
 *
 * This mode draws the object as wireframe model. To achieve this,
 * the outline of each polygon is drawn. The wireframe color can 
 * be changed through the default color argument.
 *
 * @see IRenderMode
 * @author M. Caputo
 * @date 2010-03-26
 */

#ifndef WIREFRAME_MODE_H
#define WIREFRAME_MODE_H

#include "IRenderMode.h"

class WireframeMode : public IRenderMode
{
	Q_DECLARE_TR_FUNCTIONS(WireframeMode)

public:

	QString name() const override;
	void setSettings() override;
	void unsetSettings() override;
	void draw(const IScene *scene, const QColor *defaultColor) override;
};

#endif
