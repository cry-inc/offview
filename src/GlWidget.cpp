#include "GlWidget.h"
#include "WireframeMode.h"
#include "DotMode.h"
#include "FlatShadedMode.h"
#include "SmoothShadedMode.h"
#include "ColoredMode.h"

GlWidget::GlWidget(QWidget *parent): QGLWidget(parent)
{
	// Enable SuperSampling for all view modes
	QGLWidget::setFormat(QGLFormat(QGL::SampleBuffers));

	scene = nullptr;
	activeMode = 0;

	renderModes.append(new WireframeMode());
	renderModes.append(new DotMode());
	renderModes.append(new FlatShadedMode());
	renderModes.append(new SmoothShadedMode());
	renderModes.append(new ColoredMode());
	
	// Set here the best fitting render modes
	uncoloredMode = 3; // Smooth Shaded Mode
	coloredMode = 4; // Colored Mode
	
	// Enables keyboard events for this widget
	setFocusPolicy(Qt::ClickFocus);
	
	reset();
}

GlWidget::~GlWidget()
{
	for(int i=0; i<renderModes.size(); i++) {
		delete renderModes[i];
	}
}

void GlWidget::initializeGL()
{
	renderModes[activeMode]->setSettings();
}

void GlWidget::resizeGL(int w, int h)
{
	// Fixed width and variable height:
	float width = 1.0;
	float height = (float)h / (float)w;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	width /= 3.0;
	height /= 3.0;
	
	// Set frustum with near and far clipping plane
	glFrustum(-width, +width, -height, +height, 0.5, 100.0);
	
	// Move the viewpoint by 3 units on the z axis so the camera
	// will hopefully not be inside the scene object
	glTranslatef(0.0, 0.0, -3.0);
	
	glMatrixMode(GL_MODELVIEW);
}

void GlWidget::paintGL()
{
	glClearColor(
		bgColor.redF(),
		bgColor.greenF(),
		bgColor.blueF(),
		bgColor.alphaF()
	);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScalef(scale, scale, scale);
	glTranslatef(xTrans, yTrans, 0.0);
	glRotatef(xRot, 1.0, 0.0, 0.0);
	glRotatef(yRot, 0.0, 1.0, 0.0);
	
	if (showAxes) {
		drawAxes();
	}
	
	int lines = 5;
	float stepSize = 0.2f;
	
	if (showPlanes[0]) {
		drawXzPlane(lines, stepSize, planeColor);
	}
	if (showPlanes[1]) {
		drawXyPlane(lines, stepSize, planeColor);
	}
	if (showPlanes[2]) {
		drawYzPlane(lines, stepSize, planeColor);
	}
	
	if (scene) {
		// Backup the current model view matrix
		glPushMatrix();

		// Scale to window size and move to center
		glScalef(modelScale, modelScale, modelScale);
		glTranslatef(modelOffset[0], modelOffset[1], modelOffset[2]);

		// Draw our scene
		renderModes[activeMode]->draw(scene, &color);

		// Restore the old model view matrix
		glPopMatrix();
	}
}

void GlWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
}

void GlWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		// Left mouse button is pressed: Rotate around x and y
		xRot += static_cast<float>(dy)/2.0;
		yRot += static_cast<float>(dx)/2.0;
	} else if (event->buttons() & Qt::RightButton) {
		// Right mouse button is pressed: Translate x and y
		xTrans += (static_cast<double>(dx)/200.0);
		yTrans += -(static_cast<double>(dy)/200.0);
	}

	// Remember the last position
	lastPos = event->pos();

	// Call update to apply and paint the changes
	updateGL();
}

void GlWidget::wheelEvent(QWheelEvent *event)
{	
	if (event->delta() > 0 && scale < 20) {
		scale += 0.1 * scale;
	} else if (event->delta() < 0 && scale > 0.1) {
		scale -= 0.1 * scale;
	}

	// call update to paint the changes we made here
	updateGL();
}

void GlWidget::drawAxes()
{
	glLineWidth(3.0);
	glBegin( GL_LINES );	
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);
		
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);	
	glEnd();
	glLineWidth(1.0);
}

void GlWidget::drawXzPlane(int lines, float stepSize, const QColor & c)
{
	glColor3f(c.redF(), c.greenF(), c.blueF());
	glBegin(GL_LINES);
	for(int i = -lines; i <= lines; i++) {
		glVertex3f(stepSize*i, 0, -lines*stepSize);
		glVertex3f(stepSize*i, 0, lines*stepSize);
	}
	for(int i = -lines; i <= lines; i++) {
		glVertex3f(-lines*stepSize, 0, stepSize*i);
		glVertex3f(lines*stepSize, 0, stepSize*i);
	}	
	glEnd();
}

void GlWidget::drawXyPlane(int lines, float stepSize, const QColor & c)
{
	glColor3f(c.redF(), c.greenF(), c.blueF());
	glBegin(GL_LINES);
	for(int i=-lines; i<=lines; i++) {
		glVertex3f(stepSize*i, -lines*stepSize, 0);
		glVertex3f(stepSize*i, lines*stepSize, 0);
	}
	for(int i=-lines; i<=lines; i++) {
		glVertex3f(-lines*stepSize, stepSize*i, 0);
		glVertex3f(lines*stepSize, stepSize*i, 0);
	}	
	glEnd();
}

void GlWidget::drawYzPlane(int lines, float stepSize, const QColor & c)
{
	glColor3f(c.redF(), c.greenF(), c.blueF());
	glBegin(GL_LINES);
	for(int i=-lines; i<=lines; i++) {
		glVertex3f(0, stepSize*i, -lines*stepSize);
		glVertex3f(0, stepSize*i, lines*stepSize);
	}
	for(int i=-lines; i<=lines; i++) {
		glVertex3f(0, -lines*stepSize, stepSize*i);
		glVertex3f(0, lines*stepSize, stepSize*i);
	}	
	glEnd();
}

void GlWidget::setScene(IScene *scene)
{
	this->scene = scene;
	calculateOffsetAndScale();
	reset();
	updateGL();
}

void GlWidget::calculateOffsetAndScale()
{
	if (!scene || scene->verticesCount() == 0) {
		return;
	}

	// search the x, y and z min + max values a.k.a bounding box
	const float *data = scene->vertex(0)->vertex();
	float maxX = data[0], minX = data[0];
	float maxY = data[1], minY = data[1];
	float maxZ = data[2], minZ = data[2];
	int verticesCount = scene->verticesCount();
	for(int i=1; i<verticesCount; i++) {
		data = scene->vertex(i)->vertex();
		if (data[0] > maxX) { maxX = data[0]; }
		if (data[0] < minX) { minX = data[0]; }
		if (data[1] > maxY) { maxY = data[1]; }
		if (data[1] < minY) { minY = data[1]; }
		if (data[2] > maxZ) { maxZ = data[2]; }
		if (data[2] < minZ) { minZ = data[2]; }
	}
	
	// Model dimensions
	float widthX = maxX-minX;
	float widthY = maxY-minY;
	float widthZ = maxZ-minZ;

	// This formula is from the offviwer at http://shape.cs.princeton.edu/benchmark/ 
	modelScale = 2.0/sqrt(widthX*widthX + widthY*widthY + widthZ*widthZ);

	// Center coordinates of our model
	modelOffset[0] = -(maxX-widthX/2.0);
	modelOffset[1] = -(maxY-widthY/2.0);
	modelOffset[2] = -(maxZ-widthZ/2.0);
}

void GlWidget::setBackgroundColor(const QColor& c)
{
	bgColor = c;
	updateGL();
}

const QColor& GlWidget::backgroundColor()
{
	return bgColor;
}

void GlWidget::setObjectColor(const QColor& c)
{
	color = c;
	updateGL();
}

const QColor& GlWidget::objectColor()
{
	return color;
}

void GlWidget::setXzPlane(bool status)
{
	showPlanes[0] = status;
	updateGL();
}

bool GlWidget::xzPlane()
{
	return showPlanes[0];
}

void GlWidget::setXyPlane(bool status)
{
	showPlanes[1] = status;
	updateGL();
}

bool GlWidget::xyPlane()
{
	return showPlanes[1];
}

void GlWidget::setYzPlane(bool status)
{
	showPlanes[2] = status;
	updateGL();
}

bool GlWidget::yzPlane()
{
	return showPlanes[2];
}

void GlWidget::setAxes(bool status)
{
	showAxes = status;
	updateGL();
}

bool GlWidget::axes()
{
	return showAxes;
}

void GlWidget::reset()
{
	showAxes = true;

	showPlanes[0] = true;
	showPlanes[1] = false;
	showPlanes[2] = false;

	// Reset the colors
	color = QColor(160, 160, 160);
	bgColor = QColor("black");
	planeColor = QColor(100, 100, 100);

	// Reset scale, rotation and translation values
	scale = 1.0f;

	xRot = 0.0f;
	yRot = 0.0f;

	xTrans = 0.0f;
	yTrans = 0.0f;
	
	updateGL();
}

QStringList GlWidget::listRenderModes()
{
	QStringList list;
	for(int i = 0; i < renderModes.size(); i++) {
		list.append(renderModes[i]->name());
	}
	return list;
}

int GlWidget::renderMode()
{
	return activeMode;
}

int GlWidget::modeForColoredScenes()
{
	return coloredMode;
}

int GlWidget::modeForUncoloredScenes()
{
	return uncoloredMode;
}

void GlWidget::setRenderMode(int m)
{
	renderModes[activeMode]->unsetSettings();
	activeMode = m;
	renderModes[activeMode]->setSettings();
	updateGL();
}

QString GlWidget::renderModeName()
{
	return renderModes[activeMode]->name();
}
