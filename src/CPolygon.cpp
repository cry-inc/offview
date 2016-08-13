#include <cmath>
#include "CPolygon.h"

CPolygon::CPolygon()
{
	_normal[0] = 1.0f;
	_normal[1] = 0.0f;
	_normal[2] = 0.0f;

	_colored = false;
}

void CPolygon::addVertex(CVertex* vertex)
{
	_vertices.append(vertex);
}

size_t CPolygon::vertexCount() const
{
	return _vertices.size();
}

const CVertex* CPolygon::vertex(int i) const
{
	return _vertices[i];
}

void CPolygon::setColor(const QColor& color)
{
	_colored = true;
	_color = color;
}

bool CPolygon::isColored() const
{
	return _colored;
}

const QColor& CPolygon::color() const
{
	return _color;
}

void CPolygon::calculateNormal()
{
	if (_vertices.size() < 3u) {
		return;
	}
	
	float a[3], b[3];

	// a (vector from second to first vertex)
	a[0] = _vertices[0]->x() - _vertices[1]->x();
	a[1] = _vertices[0]->y() - _vertices[1]->y();
	a[2] = _vertices[0]->z() - _vertices[1]->z();

	// b (vector from third to second vertex)
	b[0] = _vertices[1]->x() - _vertices[2]->x();
	b[1] = _vertices[1]->y() - _vertices[2]->y();
	b[2] = _vertices[1]->z() - _vertices[2]->z();

	// calculate the cross product for a and b
	_normal[0] = (a[1] * b[2]) - (a[2] * b[1]);
	_normal[1] = (a[2] * b[0]) - (a[0] * b[2]);
	_normal[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

const float* CPolygon::normal() const
{
	return _normal;
}
