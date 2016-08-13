#pragma once

#include "IRenderMode.h"

/**
 * @brief Vertex dot render mode
 *
 * This mode does not draw the model itself, but its vertices
 * in form of dots. The size of the dots is fixed, but the
 * color can be changed through the default color argument.
 *
 * @see IRenderMode
 * @author M. Caputo
 * @date 2010-03-26
 */
class DotMode : public IRenderMode
{
	Q_DECLARE_TR_FUNCTIONS(DotMode)

public:
	QString name() const override;

	void setSettings() override;

	void unsetSettings() override;

	void draw(const IScene *scene, const QColor *defaultColor) override;
};
