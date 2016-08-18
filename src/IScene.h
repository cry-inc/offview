#pragma once

#include <QColor>

#include "CPolygon.h"

/**
 * @brief The abstract scene interface
 *
 * Provides an abstract interface for models/scenes. Each scene consists of
 * polygons (CPolygon) and each polygon consists of vertices (CVertex).
 * Every polygon or vertex can has its own color.
 *
 * @see CPolygon
 * @see CVertex
 * @author M. Caputo
 * @date 2010-03-17
 */
class IScene
{
public:
	virtual ~IScene() {}

	/**
	 * @brief Is the scene colored?
	 *
	 * This method returns true, if the scene is colored.
	 * Important: A scene is colored when at least one polygon or
	 * vertex of the entire scene is colored! Only if not 
	 * a single vertex is colored, this returns false!
	 *
	 * @return True, if the scene has at least one colored polygon or vertex
	 */
	virtual bool isColored() const = 0;	

	/**
	 * @brief Returns the number of polygons
	 * @return Number of polygons
	 */
	virtual int polygonsCount() const = 0;
	
	/**
	 * @brief Getter for the polygons
	 *
	 * Returns a polygon, i is a number from 0 to n-1
	 * where n is the number of polygons.
	 *
	 * @param [in] i Number of the polygon
	 * @return A constant reference to the selected polygon
	 */
	virtual const CPolygon* polygon(int i) const = 0;
	
	/**
	 * @brief Returns the number of vertices
	 * @return Number of vertices
	 */
	virtual int verticesCount() const = 0;
	
	/**
	 * @brief Getter for the vertices
	 *
	 * Returns a vertex, i is a number from 0 to n-1
	 * where n is the number of vertices.
	 *
	 * @param [in] i Number of the vertex
	 * @return A constant reference to the selected vertex
	 */
	virtual const CVertex* vertex(int i) const = 0;
	
};
