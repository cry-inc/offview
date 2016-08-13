#include <QtOpenGL>
#include "DotMode.h"

QString DotMode::name() const
{
	return tr("Dot Mode");
}

void DotMode::setSettings()
{
	glEnable(GL_DEPTH_TEST);
	glPointSize(2.0);
}

void DotMode::unsetSettings()
{
	glPointSize(1.0);
	glDisable(GL_DEPTH_TEST);
}

void DotMode::draw(const IScene *scene, const QColor *defaultColor)
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
			glBegin( GL_POINTS );
			int vc = scene->verticesCount();
			for(int i=0; i<vc; i++) {
				const CVertex *vert = scene->vertex(i);
				const float *data = vert->vertex();
				glVertex3fv(data);
			}
			glEnd();
		glEndList ();
		oldScene = scene;
	}
	
	// Draw the object
	glCallList(displayList);
}
