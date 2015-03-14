/**
 * @file CVertex.cpp
 * @see CVertex
 * @author M. Caputo
 * @date 2010-03-17
 */

#include "CVertex.h"

/**
 * @brief Default constructor
 *
 * Default constructor for an (0|0|0) vertex without a color value.
 */
CVertex::CVertex()
{
	vertexData[0] = 0;
	vertexData[1] = 0;
	vertexData[2] = 0;
	nVector[0] = 0;
	nVector[1] = 0;
	nVector[2] = 1;
	colored = false;
}

/**
 * @brief Constructor with coordinate values
 *
 * Constructor for an (x|y|z) vertex without a color value.
 *
 * @param [in] x X coordinate value
 * @param [in] y Y coordinate value
 * @param [in] z Z coordinate value
 */
CVertex::CVertex(float x, float y, float z)
{
	vertexData[0] = x;
	vertexData[1] = y;
	vertexData[2] = z;
	nVector[0] = 0;
	nVector[1] = 0;
	nVector[2] = 1;
	colored = false;
}

/**
 * @brief Sets the vertex color
 * @param [in] color The new vertex color
 */
void CVertex::setColor(const QColor & color)
{
	colored = true;
	objectColor = color;
}

/**
 * @brief Getter for the vertex data
 *
 * Returns a pointer to an array of the three float values for x, y and z
 *
 * @return Pointer to an array of float values with x, y and z
 */
const float* CVertex::vertex() const
{
	return vertexData;
}

/**
 * @brief Getter for the X coordinate value
 * @return Value of the X coordinate
 */
float CVertex::x() const
{
	return vertexData[0];
}

/**
 * @brief Getter for the Y coordinate value
 * @return Value of the Y coordinate
 */
float CVertex::y() const
{
	return vertexData[1];
}

/**
 * @brief Getter for the Z coordinate value
 * @return Value of the Z coordinate
 */
float CVertex::z() const
{
	return vertexData[2];
}

/**
 * @brief Is the vertex colored?
 * @return True, if the vertex is colored
 */
bool CVertex::isColored() const
{
	return colored;
}

/**
 * @brief Getter for the vertex color
 *
 * Getter method for the vertex color.
 * To find out whether the vertex is colored or not, use
 * isColored() or check the return value with color.isValid()!
 *
 * @return Vertex color
 */
const QColor & CVertex::color() const
{
	return objectColor;
}

/**
 * @brief Sets the normal vector
 *
 * @param [in] x The x component of the normal vector
 * @param [in] y The y component of the normal vector
 * @param [in] z The z component of the normal vector
 */
void CVertex::setNormalVector(float x, float y, float z)
{
	nVector[0] = x;
	nVector[1] = y;
	nVector[2] = z;
}

/**
 * @brief Gets the normal vector
 *
 * This will return the normal vector of the vertex.
 * Before you can use this method, you must set
 * the normal vector with setNormalVector()!
 * Be careful, the calculated values are in most cases
 * NOT normalized! The default vector is (0|0|1).
 *
 * @return The normal vector
 */
const float* CVertex::normalVector() const
{
	return nVector;
}
