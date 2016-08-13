#pragma once

#include <QtCore>
#include <QtOpenGL>

#include "IScene.h"
#include "IRenderMode.h"

/**
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
class GlWidget : public QGLWidget
{
Q_OBJECT

public:
	/**
	 * @brief Constructor of class GlWidget that holds the OpenGL scence.
	 */
	GlWidget(QWidget *parent = nullptr);

	/**
	 * @brief Destructor of class GlWidget.
	 */
	~GlWidget();
	
	/**
	 * @brief Sets the object that is rendered.
	 */
	void setScene(IScene* scene);
	
	/**
	 * @brief Sets the background color.
	 */
	void setBackgroundColor(const QColor& color);

	/**
	 * @brief Returns the current background color.
	 */
	const QColor& backgroundColor();
	
	/**
	 * @brief Sets the new object color.
	 */
	void setObjectColor(const QColor& color);

	/**
	 * @brief Returns the current object color.
	 */
	const QColor& objectColor();
	
	/**
	 * @brief Enables/disables the XZ plane.
	 */
	void setXzPlane(bool status);

	/**
	 * @brief Enables/disables the XY plane.
	 */
	void setXyPlane(bool status);

	/**
	 * @brief Enables/disables the YZ plane.
	 */
	void setYzPlane(bool status);
	
	/**
	 * @brief Returns true if the XZ plane is enabled.
	 */
	bool xzPlane();

	/**
	 * @brief Returns true if the XY plane is enabled.
	 */
	bool xyPlane();

	/**
	 * @brief Returns true if the YZ plane is enabled.
	 */
	bool yzPlane();
	
	/**
	 * @brief Enabled/disables the XYZ axes.
	 */
	void setAxes(bool status);

	/**
	 * @brief Returns true if the XYZ axes are enabled.
	 */
	bool axes();
	
	/**
	 * @brief Returns a list with the names of all available render modes.
	 */
	QStringList listRenderModes();

	/**
	 * @brief Sets a new render mode by its index number.
	 */
	void setRenderMode(int newRenderMode);

	/**
	 * @brief Returns the index number of the current render mode.
	 */
	int renderMode();

	/**
	 * @brief Returns the name of the currently active render mode.
	 */
	QString renderModeName();
	
	/**
	 * @brief Returns the index number of the render mode best suitable for colored scenes.
	 */
	int modeForColoredScenes();

	/**
	 * @brief Returns the index number of the render mode best suitable for uncolored scenes.
	 */
	int modeForUncoloredScenes();
	
	/**
	 * @brief Resets the camera, zoom, colors axes and planes to their default values.
	 */
	void reset();
	
protected:
	/**
	 * @brief Called by the GlWidget to run the OpenGL initialization.
	 */
	void initializeGL() override;

	/**
	 * @brief Called by the GlWidget when the OpenGL context is resized.
	 */
	void resizeGL(int width, int height) override;

	/**
	 *	@brief Called by the GlWidget to draw the OpenGL context.
	 */
	void paintGL() override;

	/**
	 *	@brief Processes mouse press events of the GlWidget for rotation and translation.
	 */
	void mousePressEvent(QMouseEvent *event) override;

	/**
	 *	@brief Processes mouse move events of the GlWidget for rotation and translation.
	 */
	void mouseMoveEvent(QMouseEvent *event) override;
	
	/**
	 *	@brief Processes mouse wheel events of the GlWidget for zooming
	 */
	void wheelEvent(QWheelEvent *event) override;

private:
	// Do not allow copy constructor and the assignment operator
	GlWidget(const GlWidget & other);
	GlWidget& operator=(const GlWidget& other);
	
	/**
	 * @brief Draws the coordinate axes to the OpenGL context.
	 */
	void drawAxes();

	/**
	 * @brief Draws the XZ plane to the OpenGL context.
	 */
	void drawXzPlane(int lines, float stepSize, const QColor & c);

	/**
	 * @brief Draws the XY plane to the OpenGL context.
	 */
	void drawXyPlane(int lines, float stepSize, const QColor & c);

	/**
	 * @brief Draws the YZ plane to the OpenGL context.
	 */
	void drawYzPlane(int lines, float stepSize, const QColor & c);

	/**
	 * @brief Calculate the model offset and scale values.
	 *
	 * The values calculated here are used by paintGL() to center our model
	 * and scale it so it fits the window.
	 */
	void calculateOffsetAndScale();

	/**
	 * @brief Holds all available render modes.
	 */
	QVector<IRenderMode*> renderModes;

	/**
	 * @brief Index number of the currently active render mode.
	 */
	int activeMode;

	/**
	 * @brief Index number of the best available mode for colored scenes.
	 *
	 * This variable should be set in the constructor after the render modes have been created!
	 *
	 * @see modeForColoredScenes()
	 */
	int coloredMode;

	/**
	 * @brief Index number of the best available mode for uncolored scenes.
	 *
	 * This variable should be set in the constructor after the render modes have been created!
	 *
	 * @see modeForUncoloredScenes()
	 */
	int uncoloredMode;

	/**
	 * @brief Pointer to the currently active object.
	 */
	IScene* scene;

	/**
	 * @brief Last Position where a mouse event happend.
	 */
	QPoint lastPos;

	/**
	 * @brief Current background color.
	 */
	QColor bgColor;
	
	/**
	 * @brief Current plane line color.
	 */
	QColor planeColor;

	/**
	 * @brief Color of the object.
	 */
	QColor color;

	/**
	 * @brief Are the axes shown or not?
	 */
	bool showAxes;

	/**
	 * @brief Are the planes visible?
	 */
	bool showPlanes[3]; // XZ, XY, ZY

	/**
	 * @brief Value for the model scale.
	 */
	float scale;

	/**
	 * @brief Value for moving the model along the X axis of the screen.
	 */
	float xTrans;

	/**
	 * @brief Value for moving the model along the Y axis of the screen.
	 */
	float yTrans;

	/**
	 * @brief Value for rotating the model around the vertical axis of the screen.
	 */
	float xRot;
	
	/**
	 * @brief Value for rotating the model around the horizontal axis of the screen.
	 */
	float yRot;

	/**
	 * @brief The offset values which are needed to center our model.
	 * @see calculateOffsetAndScale()
	 */
	float modelOffset[3];
	
	/**
	 * @brief The scale value which is needed to fit our model to the camera.
	 * @see	calculateOffsetAndScale()
	 */
	float modelScale;
};
