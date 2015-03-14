/**
 * @class CVertex
 * @brief A colored vertex
 *
 * A vertex with an optional color value. It consists of three float values
 * for the x, y and z axis.
 *
 * @author M. Caputo
 * @date 2010-03-03
 */

#ifndef CVERTEX_H
#define CVERTEX_H

#include <QColor>

class CVertex
{

public:
	CVertex();
	CVertex(float x, float y, float z);

	const float* vertex() const;
	float x() const;
	float y() const;
	float z() const;
	void setColor(const QColor & color);
	bool isColored() const;
	const QColor & color() const;
	void setNormalVector(float x, float y, float z);
	const float* normalVector() const;

private:
	/**
	 * @brief The optional vertex color
	 */
	QColor objectColor;
	
	/**
	 * @brief Is the vertex colored?
	 */
	bool colored;
	
	/**
	 * @brief The vertex x, y and z data
	 */
	float vertexData[3];
	
	/**
	 * @brief The normal vector for the vertex
	 */
	float nVector[3];

};

#endif
