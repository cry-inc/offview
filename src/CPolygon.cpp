/**
 * @file CPolygon.cpp
 * @see CPolygon
 * @author M. Caputo
 * @date 2010-03-17
 */

#include <cmath>
#include "CPolygon.h"

/**
 * @brief Default constructor
 *
 * Default constructor for an empty polygon without any color or vertices.
 */
CPolygon::CPolygon()
{
	nVector[0] = 0;
	nVector[1] = 0;
	nVector[2] = 1;
	colored = false;
}

/**
 * @brief Constructor with color
 *
 * Constructor for an empty polygon with the specified color value.
 *
 * @param [in] color Polygon color
 */
CPolygon::CPolygon(const QColor & color)
{
	nVector[0] = 0;
	nVector[1] = 0;
	nVector[2] = 1;
	colored = true;
	objectColor = color;
}

/**
 * @brief Adds a new vertex to the polygon
 * @param [in] vertex A vertex which should be added to the polygon
 * @see CVertex
 */
void CPolygon::addVertex(CVertex *vertex)
{
	vertices.append(vertex);
}

/**
 * @brief Returns the number of vertices
 * @return Number of vertices
 */
int CPolygon::verticesCount() const
{
	return vertices.size();
}

/**
 * @brief Getter for the vertices
 *
 * Returns a vertex, i is a number from 0 to n-1
 * where n is the number of vertices.
 *
 * @param [in] i Number of the vertex
 * @return A constant reference to the selected vertex
 */
const CVertex* CPolygon::vertex(int i) const
{
	return vertices[i];
}

/**
 * @brief Sets the polygon color
 * @param [in] color The new polygon color
 */
void CPolygon::setColor(const QColor & color)
{
	colored = true;
	objectColor = color;
}

/**
 * @brief Is the polygon colored?
 * @return True, if the polygon is colored
 */
bool CPolygon::isColored() const
{
	return colored;
}

/**
 * @brief Getter for the polygon color
 *
 * Getter method for the polygon color.
 * To find out whether the polygon is colored or not, use
 * isColored() or check the return value with color.isValid()!
 *
 * @return Polygon color
 */
const QColor & CPolygon::color() const
{
	return objectColor;
}

/**
 * @brief Caclulate the normal vector
 *
 * This will calculate the normal vector of a polygon.
 * If a polygon has less than 3 vertices, this will fail
 * and the default vector (0|0|1) will be remain unchanged!
 */
void CPolygon::calculateNormalVector()
{
	if (vertices.size() < 3) {
		return;
	}
	
	float a[3], b[3];

	// a
	a[0] = vertices[0]->x() - vertices[1]->x();
	a[1] = vertices[0]->y() - vertices[1]->y();
	a[2] = vertices[0]->z() - vertices[1]->z();

	// b
	b[0] = vertices[1]->x() - vertices[2]->x();
	b[1] = vertices[1]->y() - vertices[2]->y();
	b[2] = vertices[1]->z() - vertices[2]->z();

	// calculate the cross product
	nVector[0] = (a[1] * b[2]) - (a[2] * b[1]);
	nVector[1] = (a[2] * b[0]) - (a[0] * b[2]);
	nVector[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

/**
 * @brief Get the normal vector
 *
 * This will return the normal vector of the polygon.
 * Before you can use this method, you must calculate
 * the normal vector with calculateNormalVector()!
 * Be careful, the calculated values are in most cases
 * NOT normalized! The default vector is (0|0|1).
 *
 * @return The normal vector
 */
const float* CPolygon::normalVector() const
{
	return nVector;
}
