// SHPoint.cpp: implementation of the SHPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "sh_point.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SHPoint::SHPoint(float x, float y)
{
	this->x = x;
	this->y = y;
}

SHPoint::~SHPoint()
{

}

float SHPoint::getX()
{
	return x;
}

float SHPoint::getY()
{
	return y;
}

void SHPoint::setX(float x)
{
	this->x = x;
}

void SHPoint::setY(float y)
{
	this->y = y;
}