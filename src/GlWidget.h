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
	 * @brief	Value for scaling the model.
	 */
	float scale;

	/**
	 * @brief	Value for moving the model along the x-axis of the screen.
	 */
	float xTrans;

	/**
	 * @brief	Value for moving the model along the y-axis of the screen.
	 */
	float yTrans;

	/**
	 * @brief	Value for rotating the model around the vertical axis of the screen.
	 */
	float xRot;
	
	/**
	 * @brief	Value for rotating the model around the horizontal axis of the screen.
	 */
	float yRot;

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
