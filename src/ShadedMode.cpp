#include <QtOpenGL>
#include "ShadedMode.h"

ShadedMode::ShadedMode(bool smoothShaded, bool colored, bool specular)
{
	this->smoothShaded = smoothShaded;
	this->colored = colored;
	this->specular = specular;
	
	oldScene = 0;
	displayList = 0;
}

void ShadedMode::setSettings()
{	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
}

void ShadedMode::unsetSettings()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_NORMALIZE);
}

void ShadedMode::draw(const IScene *scene, const QColor *color)
{	
	// If the scene or the color has changed, we need to create an new display list
	if (displayList == 0 || oldScene != scene || oldColor != *color) {
		createDisplayList(scene, color);
		oldScene = scene;
		oldColor = *color;
	}
	
	// Specular settings
	if (specular) {
		const float specularColor[] = { 1.0, 1.0, 1.0, 1.0 };
		const float specularReflection[] = { 0.5, 0.5, 0.5, 1.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularReflection);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 50);
	} else {
		const float specularColor[] = { 0.0, 0.0, 0.0, 0.0 };
		const float specularReflection[] = { 0.0, 0.0, 0.0, 0.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularReflection);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	}
	
	// light settings
	const float lightColor[4] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Turn on the light only for the object on
	glEnable(GL_LIGHTING);
	glCallList(displayList);
	glDisable(GL_LIGHTING);
}

void ShadedMode::createDisplayList(const IScene *scene, const QColor *color)
{
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);
		int cp = scene->polygonsCount();
		for(int i=0; i<cp; i++) {
			const CPolygon *poly = scene->polygon(i);
			size_t cv = poly->vertexCount();
			const float *nv = 0;
			if (!smoothShaded) {
				nv = poly->normal();
			}
			glBegin( GL_POLYGON );
				for(int j=0; j<cv; j++) {
					const CVertex *vert = poly->vertex(j);
					const float *data = vert->vertex();

					// If the vertex is colored, this overrides all other colors
					if (colored && vert->isColored()) {
						glColor4f(
							vert->color().redF(),
							vert->color().greenF(),
							vert->color().blueF(),
							vert->color().alphaF()
						);
					// A polygon color is replacing the default object color
					} else if (colored && poly->isColored()) {
						glColor4f(
							poly->color().redF(),
							poly->color().greenF(),
							poly->color().blueF(),
							poly->color().alphaF()
						);
					// The default color is our failsafe fallback solution :)
					} else {
						glColor4f(
							color->redF(),
							color->greenF(),
							color->blueF(),
							color->alphaF()
						);
					}
					
					if (smoothShaded) {
						nv = vert->normal();
					}
					glNormal3fv(nv);
					glVertex3fv(data); 
				}
			glEnd();
		}
	
	glEndList ();
}
