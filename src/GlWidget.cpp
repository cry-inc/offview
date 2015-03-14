/**
 * @file GlWidget.cpp
 * @see GlWidget
 * @author D. Fritz
 * @author M. Caputo
 * @date 2010-02-25
 */

#include "GlWidget.h"
#include "WireframeMode.h"
#include "DotMode.h"
#include "FlatShadedMode.h"
#include "SmoothShadedMode.h"
#include "ColoredMode.h"

/**
 * @brief Constructor of class GlWidget.
 *
 * The class inherits QGlWidget (public).
 * Creates an object of class GlWidget.
 *
 * @param[in] parent Parent widget (default = 0).
 */
GlWidget::GlWidget(QWidget *parent): QGLWidget(parent)
{
	// Enable SuperSampling for all view modes
	QGLWidget::setFormat(QGLFormat(QGL::SampleBuffers));

	scene = 0;
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
	
	resetView();
}

/**
 * @brief Destructor of class GlWidget
 *
 * Destroys an object of the class GlWidget.
 */
GlWidget::~GlWidget()
{
	for(int i=0; i<renderModes.size(); i++) {
		delete renderModes[i];
	}
}

/**
 * @brief	Initializes the active render mode.
 */
void GlWidget::initializeGL()
{
	renderModes[activeMode]->setSettings();
}

/**
 * @brief Resizes the GlWidget
 *
 * The function readjustes the size of the GlWidget whenever needed.
 *
 * @param	w = Integer-Value of new width
 * @param	h = Integer-Value of new heigth
 */
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

/**
 *	@brief	Draws the GlWidget
 *
 * Central function that draws the 3D-Object, axes and planes.
 */
void GlWidget::paintGL()
{
	glClearColor(
		bgColor.redF(),
		bgColor.greenF(),
		bgColor.blueF(),
		bgColor.alphaF()
	);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (isBeingReset) {
		glLoadIdentity();
		isBeingReset = false;
	}

	if (mustScale) {
		glScalef(scale, scale, scale);
		mustScale = false;
	}
	
	if (mustTranslate) {
		glTranslatef(xTrans, yTrans, zTrans);
		mustTranslate = false;
	}
	
	if (mustRotate) {
		glRotatef(xRot, 1.0, 0.0, 0.0);
		glRotatef(yRot, 0.0, 1.0, 0.0);
		glRotatef(zRot, 0.0, 0.0, 1.0);
		mustRotate = false;
	}
	
	if (showAxes) {
		drawAxes();
	}
	
	int lines = 5;
	float stepSize = 0.2;
	
	if (showPlanes[0]) {
		drawXzPlane(lines, stepSize, gridColor);
	}
	if (showPlanes[1]) {
		drawXyPlane(lines, stepSize, gridColor);
	}
	if (showPlanes[2]) {
		drawYzPlane(lines, stepSize, gridColor);
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

/**
 *	@brief	Processes mouse clicks.
 *
 * Updates lastPos to the current position.
 *
 * @param	event Event that occured
 */
void GlWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
}

/**
 *	@brief	Processes mouse movements.
 *
 * Depending on which mouse and/or keyboard
 *	buttons are pressed, the variables, paintGL uses to manipulate the
 * object, are changed.
 *
 * @param	event Event that occured
 */
void GlWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		// Left mouse button is pressed: Rotate
		xRot = 0.0; yRot = 0.0; zRot = 0.0;
		mustRotate = true;
		if (xDown) {
			xRot = static_cast<float>(dy)/2.0;
		} else if (yDown) {
			yRot = static_cast<float>(dx)/2.0;
		} else if (zDown) {
			zRot = static_cast<float>(dy)/2.0;
		} else {
			// If no key is pressed, we rotate around x and y
			xRot = static_cast<float>(dy)/2.0;
			yRot = static_cast<float>(dx)/2.0;
		}		
	} else if (event->buttons() & Qt::RightButton) {
		// Right mouse button is pressed: Translate
		xTrans = 0.0; yTrans = 0.0; zTrans = 0.0;
		mustTranslate = true;
		if (xDown) {
			xTrans = (static_cast<double>(dx)/200.0);
		} else if (yDown) {
			yTrans = -(static_cast<double>(dy)/200.0);
		} else if (zDown) {
			zTrans = (static_cast<double>(dy)/200.0);
		} else {
			// If no key is pressed, we translate x and y
			xTrans = (static_cast<double>(dx)/200.0);
			yTrans = -(static_cast<double>(dy)/200.0);
		}
	}

	// Remember the last position
	lastPos = event->pos();

	// Call update to apply and paint the changes
	updateGL();
}

/**
 *	@brief	Processes turns of the mouse wheel
 *
 * Whenever we scroll with the mouse wheel, we zoom in/out.
 *
 * @param	event Event that occured
 */
void GlWidget::wheelEvent(QWheelEvent *event)
{	
	if (event->delta() > 0) {
		scale = 1.1;
	} else {
		scale = 0.9;
	}

	// note that we have to call 'glScalef' in 'paintGL'
	mustScale = true;

	// call update to paint the changes we made here
	updateGL();
}

/**
 * @brief	Processes keystrokes
 *
 * As the axe we are spinning around depends on the key pressed,
 * we have to note them down.
 *
 * @param	event Event that occured
 */
void GlWidget::keyPressEvent(QKeyEvent *event)
{
	switch(event->key()) {
		case Qt::Key_Y :
			yDown = true;
			break;
		case Qt::Key_X :
			xDown = true;
			break;
		case Qt::Key_Z :
			zDown = true;
			break;
		default :
			QGLWidget::keyPressEvent(event);
	}
}

/**
 * @brief	Processes key-releases
 *
 * We also have to note when keys are released!
 *
 * @param	event Event that occured
 */
void GlWidget::keyReleaseEvent(QKeyEvent *event)
{
	switch(event->key()) {
		case Qt::Key_Y :
			if(!event->isAutoRepeat()) {
				yDown = false;
			}
			break;
		case Qt::Key_X :
			if(!event->isAutoRepeat()) {
				xDown = false;
			}
			break;
		case Qt::Key_Z :
			if(!event->isAutoRepeat()) {
				zDown = false;
			}
			break;
		default :
			QGLWidget::keyReleaseEvent(event);
	}
}

/**
 * @brief	Draws the coordinate axes of the object
 */
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

/**
 * @brief	Draws the x-z-plane
 *
 * @param	lines		number that decides how many lines the plane consists of.
 * @param	stepSize	Distance between two lines
 * @param	c			color of the plane
 */
void GlWidget::drawXzPlane(int lines, float stepSize, const QColor & c)
{
	glColor3f(c.redF(), c.greenF(), c.blueF());
	glBegin( GL_LINES );
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

/**
 * @brief	Draws the x-y-plane
 *
 * @param	lines		number that decides how many lines the plane consists of.
 * @param	stepSize	Distance between two lines
 * @param	c			color of the plane
 */
void GlWidget::drawXyPlane(int lines, float stepSize, const QColor & c)
{
	glColor3f(c.redF(), c.greenF(), c.blueF());
	glBegin( GL_LINES );
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

/**
 * @brief	Draws the y-z-plane
 *
 * @param	lines		number that decides how many lines the plane consists of.
 * @param	stepSize	Distance between two lines
 * @param	c			color of the plane
 */
void GlWidget::drawYzPlane(int lines, float stepSize, const QColor & c)
{
	glColor3f(c.redF(), c.greenF(), c.blueF());
	glBegin( GL_LINES );
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

/**
 * @brief	The new value for the class attribute 'scene' is 'scene'.
 */
void GlWidget::setScene(IScene *scene)
{
	this->scene = scene;
	calculateOffsetAndScale();
	resetView();
	updateGL();
}

/**
 * @brief	Calculate the model offset and scale values
 *
 * The values calculated here are used by paintGL() to center our model
 * and scale it so it fits the window.
 */
void GlWidget::calculateOffsetAndScale()
{
	if (!scene || scene->verticesCount() < 1) {
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

/**
 * @brief	The new value for the class attribute 'bgcolor' is 'c'.
 */
void GlWidget::setBackgroundColor(const QColor & c)
{
	bgColor = c;
	updateGL();
}

/**
 * @brief	Get the background color.
 *
 * @return	Value of class attribute 'bgcolor'
 */
QColor GlWidget::backgroundColor()
{
	return bgColor;
}

/**
 * @brief	The new value for the class attribute 'color' is 'c'.
 */
void GlWidget::setObjectColor(const QColor & c)
{
	color = c;
	updateGL();
}

/**
 * @brief	Get the color of the object.
 *
 * @return	Value of class attribute 'color'
 */
QColor GlWidget::objectColor()
{
	return color;
}

/**
 * @brief	The new value for the class attribute
 *				'showPlanes' at Index 0 is 'status'.
 */
void GlWidget::setXzPlane(bool status)
{
	showPlanes[0] = status;
	updateGL();
}

/**
 * @brief	Check whether the x-z-plane is to be drawn or not.
 *
 * @return	Value of class attribute 'showPlanes' at Index 0
 */
bool GlWidget::xzPlane()
{
	return showPlanes[0];
}

/**
 * @brief	The new value for the class attribute
 *				'showPlanes' at Index 1 is 'status'.
 */
void GlWidget::setXyPlane(bool status)
{
	showPlanes[1] = status;
	updateGL();
}

/**
 * @brief	Check whether the x-y-plane is to be drawn or not.
 *
 * @return	Value of class attribute 'showPlanes' at Index 1
 */
bool GlWidget::xyPlane()
{
	return showPlanes[1];
}

/**
 * @brief	The new value for the class attribute
 *				'showPlanes' at Index 2 is 'status'.
 */
void GlWidget::setYzPlane(bool status)
{
	showPlanes[2] = status;
	updateGL();
}

/**
 * @brief	Check whether the y-z-plane is to be drawn or not.
 *
 * @return	Value of class attribute 'showPlanes' at Index 2
 */
bool GlWidget::yzPlane()
{
	return showPlanes[2];
}

/**
 * @brief	The new value for the class attribute 'showAxes' is 'status'.
 */
void GlWidget::setAxes(bool status)
{
	showAxes = status;
	updateGL();
}

/**
 * @brief	Check whether the coordinate axes of the object are to be drawn or not.
 *
 * @return	Value of class attribute 'showAxes'
 */
bool GlWidget::axes()
{
	return showAxes;
}

/**
 * @brief Resets the view
 *
 * Whenever we want to reset the view,
 * all attributes must be reset to their initial values.
 */
void GlWidget::resetView()
{
	// Set these values so that the object is correctly redrawn
	isBeingReset = true;

	xDown = false;
	yDown = false;
	zDown = false;

	mustScale = false;
	mustTranslate = false;
	mustRotate = false;

	showAxes = true;

	showPlanes[0] = true;
	showPlanes[1] = false;
	showPlanes[2] = false;

	// Reset the colors
	color = QColor(160, 160, 160);
	bgColor = QColor("black");
	gridColor = QColor(100, 100, 100);

	// Reset scale, rotate and translate values
	scale = 1.0;
	xRot = 0.0; yRot = 0.0; zRot = 0.0;
	xTrans = 0.0; yTrans = 0.0; zTrans = 0.0;
	
	updateGL();
}

/**
 * @brief	Lists the names of all render modes.
 *
 * @see		IRenderMode
 * @return	A QStringList, containing all these names.
 */
QStringList GlWidget::listRenderModes()
{
	QStringList list;
	for(int i = 0; i < renderModes.size(); i++) {
		list.append(renderModes[i]->name());
	}
	return list;
}

/**
 * @brief	Provides the active render mode
 *
 * @return	Value of class attribute 'activeMode'
 */
int GlWidget::renderMode()
{
	return activeMode;
}

/**
 * @brief	Provides the best render mode for colored scenes
 *
 * @return	Index number of the render mode
 */
int GlWidget::modeForColoredScenes()
{
	return coloredMode;
}

/**
 * @brief	Provides the best render mode for uncolored scenes
 *
 * @return	Index number of the render mode
 */
int GlWidget::modeForUncoloredScenes()
{
	return uncoloredMode;
}

/**
 * @brief	Sets a new render mode.
 *
 * The new value for the class attribute 'activeMode' is 'm'.
 * Functions to activate the new render mode and to deactivate
 * the old one are called.
 *
 * @see		IRenderMode
 * @return	no return value(void)
 */
void GlWidget::setRenderMode(int m)
{
	renderModes[activeMode]->unsetSettings();
	activeMode = m;
	renderModes[activeMode]->setSettings();
	updateGL();
}

/**
 * @brief	Provides the name of the active render mode
 *
 * @see		IRenderMode
 * @return	Name of class attribute 'activeMode'
 */
QString GlWidget::renderModeName()
{
	return renderModes[activeMode]->name();
}
