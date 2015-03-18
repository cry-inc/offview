/**
 * @file WireframeMode.cpp
 * @see WireframeMode
 * @see IRenderMode
 * @author M. Caputo
 * @date 2010-03-26
 */

#include <QtOpenGL>
#include "WireframeMode.h"

QString WireframeMode::name() const
{
	return tr("Wireframe Mode");
}

void WireframeMode::setSettings()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void WireframeMode::unsetSettings()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void WireframeMode::draw(const IScene *scene, const QColor *defaultColor)
{
	// Remeber our scene pointer and the display list id
	static const IScene *oldScene = 0;
	static unsigned int displayList = 0;
	
	// Do not place the color inside the display list because it can change!
	glColor3f(
		defaultColor->redF(),
		defaultColor->greenF(),
		defaultColor->blueF()
	);
	
	// Do we need to update the display list?
	if (oldScene != scene || displayList == 0) {
		displayList = glGenLists(1);
		glNewList(displayList, GL_COMPILE);
			int cp = scene->polygonsCount();
			for(int i=0; i<cp; i++) {
				const CPolygon *poly = scene->polygon(i);
				int cv = poly->verticesCount();
				glBegin( GL_LINE_LOOP );
					for(int j=0; j<cv; j++) {
						const CVertex *vert = poly->vertex(j);
						const float *data = vert->vertex();
						glVertex3fv(data);
					}
				glEnd();
			}
		glEndList ();
		oldScene = scene;
	}
	
	// Draw the object
	glCallList(displayList);
}
