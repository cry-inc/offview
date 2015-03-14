/**
 * @class CPolygon
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

#ifndef CPOLYGON_H
#define CPOLYGON_H

#include <QColor>
#include <QVector>

#include "CVertex.h"

class CPolygon
{

public:
	CPolygon();
	CPolygon(const QColor & color);
	
	void addVertex(CVertex *v);
	int verticesCount() const;
	const CVertex* vertex(int i) const;
	void setColor(const QColor & color);
	bool isColored() const;
	const QColor & color() const;
	void calculateNormalVector();
	const float* normalVector() const;

private:
	/**
	 * @brief The optional polygon color
	 */
	QColor objectColor;
	
	/**
	 * @brief Is the polygon colored?
	 */
	bool colored;
	
	/**
	 * @brief The polgyons vertices
	 */
	QVector<CVertex*> vertices;

	/**
	 * @brief The normal vector for the polygon surface
	 */
	float nVector[3];
};

#endif
