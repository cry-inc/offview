/**
 * @class GlWidget
 * @brief Qt widget that can display an IScene object with OpenGL
 *
 * GlWidget is inherited from QGLWidget and displays a scene
 * which implements the IScene interface.
 *
 * @author D. Fritz
 * @author M. Caputo
 * @date 2010-02-25
 * @see IScene
 */

#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QtCore>
#include <QtOpenGL>

#include "IScene.h"
#include "IRenderMode.h"

class GlWidget : public QGLWidget
{

Q_OBJECT

public:
	GlWidget(QWidget *parent = 0);
	~GlWidget();
	
	void setScene(IScene *scene);
	
	void setBackgroundColor(const QColor & color);
	QColor backgroundColor();
	
	void setObjectColor(const QColor & color);
	QColor objectColor();
	
	void setXzPlane(bool status);
	void setXyPlane(bool status);
	void setYzPlane(bool status);
	
	bool xzPlane();
	bool xyPlane();
	bool yzPlane();
	
	void setAxes(bool status);
	bool axes();
	
	QStringList listRenderModes();
	void setRenderMode(int m);
	int renderMode();
	QString renderModeName();
	
	int modeForColoredScenes();
	int modeForUncoloredScenes();
	
	void resetView();
	
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:
	void drawAxes();
	void drawXzPlane(int lines, float stepSize, const QColor & c);
	void drawXyPlane(int lines, float stepSize, const QColor & c);
	void drawYzPlane(int lines, float stepSize, const QColor & c);
	void calculateOffsetAndScale();

	// Do not allow copy constructor and the assignment operator
	GlWidget(const GlWidget & other);
	GlWidget & operator=(const GlWidget& other);
	
	/**
	 * @brief	Includes all render modes available
	 */
	QVector<IRenderMode*> renderModes;

	/**
	 * @brief	Index of the currently active render mode
	 */
	int activeMode;

	/**
	 * @brief	Best available mode for colored scenes
	 *
	 * This should contain the index number of the best available render
	 * mode for colored scenes. This variable should be set in the constructor
	 * after the render modes have been created!
	 *
	 * @see modeForColoredScenes()
	 */
	int coloredMode;

	/**
	 * @brief	Best available mode for uncolored scenes
	 *
	 * This should contain the index number of the best available render
	 * mode for uncolored scenes. This variable should be set in the constructor
	 * after the render modes have been created!
	 *
	 * @see modeForUncoloredScenes()
	 */
	int uncoloredMode;

	/**
	 * @brief	Pointer to the scene.
	 */
	IScene *scene;

	/**
	 * @brief	Last Position where any mouse event happend.
	 */
	QPoint lastPos;

	/**
	 * @brief	Current background color.
	 */
	QColor bgColor;
	
	/**
	 * @brief	Current grid line color.
	 */
	QColor gridColor;

	/**
	 * @brief	Color of the object (in case it is to be shown).
	 */
	QColor color;

	/**
	 * @brief	Are the axes shown or not?
	 */
	bool showAxes;

	/**
	 * @brief	For each of the three planes:
	 *				whether they are shown or not.
	 */
	bool showPlanes[3]; // xz, xy, zy

	/**
	 * @brief	Value for the next glScalef.
	 */
	float scale;

	/**
	 * @brief	Value for the next glTranslatef in direction of the x-axis.
	 */
	float xTrans;

	/**
	 * @brief	Value for the next glTranslatef in direction of the y-axis.
	 */
	float yTrans;

	/**
	 * @brief	Value for the next glTranslatef in direction of the z-axis.
	 */
	float zTrans;

	/**
	 * @brief	Value for the next glRotate on the x axis.
	 */
	float xRot;
	
	/**
	 * @brief	Value for the next glRotate on the y axis.
	 */
	float yRot;
	
	/**
	 * @brief	Value for the next glRotate on the z axis.
	 */
	float zRot;

	/**
	 * @brief	Is the key 'x' pressed down?
	 */
	bool xDown;
	
	/**
	 * @brief	Is the key 'y' pressed down?
	 */
	bool yDown;
	
	/**
	 * @brief	Is the key 'z' pressed down?
	 */
	bool zDown;

	/**
	 * @brief	Do we have to reset the whole view?
	 */
	bool isBeingReset;

	/**
	 * @brief	Do we have to call glScalef at the next update(paintGL)?
	 */
	bool mustScale;

	/**
	 * @brief	Do we have to call glTranslate at the next update(paintGL)?
	 */
	bool mustTranslate;
	
	/**
	 * @brief	Do we have to call glRotate at the next update(paintGL)?
	 */
	bool mustRotate;
	
	/**
	 * @brief	The offset values which are needed to center our model
	 * @see		calculateOffsetAndScale()
	 */
	float modelOffset[3];
	
	/**
	 * @brief	The scale value which is needed to resize our model
	 * @see		calculateOffsetAndScale()
	 */
	float modelScale;
};

#endif
