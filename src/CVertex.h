#pragma once

#include <QColor>

/**
 * A vertex with an optional color value.
 *
 * It consists of three float values for the x, y and z axis.
 *
 * @author M. Caputo
 * @date 2010-03-03
 */
class CVertex
{
public:
	/**
	 * Constructor with coordinate values.
	 *
	 * Constructor for an (x|y|z) vertex without a color value.
	 *
	 * @param [in] xyz XYZ coordinate values.
	 */
	CVertex(float* xyz);

	/**
	 * Getter for the vertex data.
	 *
	 * @return Pointer to an array of float values with x, y and z.
	 */
	const float* vertex() const;

	/**
	 * Getter for the X coordinate value.
	 *
	 * @return Value of the X coordinate.
	 */
	const float& x() const;

	/**
	 * Getter for the Y coordinate value.
	 *
	 * @return Value of the Y coordinate.
	 */
	const float& y() const;

	/**
	 * Getter for the Z coordinate value.
	 *
	 * @return Value of the Z coordinate.
	 */
	const float& z() const;

	/**
	 * Sets the vertex color.
	 *
	 * @param [in] color The new vertex color.
	 */
	void setColor(const QColor& color);

	/**
	 * Is the vertex colored?
	 *
	 * @return True, if the vertex is colored.
	 */
	bool isColored() const;

	/**
	 * Getter for the vertex color.
	 *
	 * To find out whether the vertex is colored or not, use
	 * isColored() or check the return value with isValid()!
	 *
	 * @return The color of the vertex. Can be an invalid color.
	 */
	const QColor& color() const;

	/**
	 * Sets the normal vector.
	 *
	 * @param [in] normal The normal vector.
	 */
	void setNormal(float* normal);

	/**
	 * Gets the normal vector.
	 *
	 * Before you can use this method, you must set
	 * the normal vector with setNormalVector()!
	 * Be careful, the calculated values are in most cases
	 * NOT normalized! The default vector is (0|0|1).
	 *
	 * @return The normal vector.
	 */
	const float* normal() const;

private:
	/**
	 * The optional vertex color.
	 */
	QColor _color;
	
	/**
	 * Is the vertex colored?
	 */
	bool _colored;
	
	/**
	 * The vertex x, y and z data.
	 */
	float _xyz[3];
	
	/**
	 * The normal vector for the vertex.
	 */
	float _normal[3];
};
