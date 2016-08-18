#pragma once

#include <QColor>
#include <QVector>

#include "CVertex.h"

/**
 * @brief A colored polygon
 *
 * A polygon with an optional color value. It consists of vertices (CVertex).
 * The number of vertices is not checked, so make sure to check the number of
 * vertices for greater than three before start drawing it!
 *
 * @see CVertex
 * @author M. Caputo
 * @date 2010-03-03
 */
class CPolygon
{
public:
	/**
	 * @brief Default constructor
	 *
	 * Default constructor for an empty polygon without any color or vertices.
	 */
	CPolygon();
	
	/**
	 * @brief Adds a new vertex to the polygon
	 * @param [in] vertex A vertex which should be added to the polygon
	 * @see CVertex
	 */
	void addVertex(CVertex* vertex);

	/**
	 * @brief Returns the number of vertices
	 * @return Number of vertices
	 */
	size_t vertexCount() const;

	/**
	 * @brief Getter for the vertices
	 *
	 * Returns a vertex, i is a number from 0 to n-1
	 * where n is the number of vertices.
	 *
	 * @param [in] i Number of the vertex
	 * @return A constant reference to the selected vertex
	 */
	const CVertex* vertex(int i) const;

	/**
	 * @brief Sets the polygon color
	 * @param [in] color The new polygon color
	 */
	void setColor(const QColor& color);

	/**
	 * @brief Is the polygon colored?
	 * @return True, if the polygon is colored
	 */
	bool isColored() const;

	/**
	 * @brief Getter for the polygon color
	 *
	 * Getter method for the polygon color.
	 * To find out whether the polygon is colored or not, use
	 * isColored() or check the return value with color.isValid()!
	 *
	 * @return Polygon color
	 */
	const QColor& color() const;

	/**
	 * @brief Caclulate the normal vector
	 *
	 * This will calculate the normal vector of a polygon.
	 * This will only use the first three vertices of the polygon.
	 * If a polygon has less than 3 vertices, this will fail
	 * and the previous normal will remain unchanged!
	 */
	void calculateNormal();

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
	const float* normal() const;

private:
	/**
	 * @brief The optional polygon color
	 */
	QColor _color;
	
	/**
	 * @brief Is the polygon colored?
	 */
	bool _colored;
	
	/**
	 * @brief The polgyons vertices
	 */
	QVector<CVertex*> _vertices;

	/**
	 * @brief The normal vector for the polygon surface
	 */
	float _normal[3];
};
