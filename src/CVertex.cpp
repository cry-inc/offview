#include "CVertex.h"

CVertex::CVertex(float* xyz)
{
	_xyz[0] = xyz[0];
	_xyz[1] = xyz[1];
	_xyz[2] = xyz[2];

	_normal[0] = 0.0f;
	_normal[1] = 0.0f;
	_normal[2] = 1.0f;

	_colored = false;
}

void CVertex::setColor(const QColor& color)
{
	_colored = true;
	_color = color;
}

const float* CVertex::vertex() const
{
	return _xyz;
}

const float& CVertex::x() const
{
	return _xyz[0];
}

const float& CVertex::y() const
{
	return _xyz[1];
}

const float& CVertex::z() const
{
	return _xyz[2];
}

bool CVertex::isColored() const
{
	return _colored;
}

const QColor & CVertex::color() const
{
	return _color;
}

void CVertex::setNormal(float* normal)
{
	_normal[0] = normal[0];
	_normal[1] = normal[1];
	_normal[2] = normal[2];
}

const float* CVertex::normal() const
{
	return _normal;
}
