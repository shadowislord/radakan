// SHPoint.h: interface for the SHPoint class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SH_POINT_HPP
#define SH_POINT_HPP

#include "object.hpp"

class SH_Point:
	public Object
{
	public:
		SH_Point (float new_x, float new_y);					//	Constructor
		virtual ~SH_Point ();									//	Destructor
		float x;
		float y;
};

#endif // SH_POINT_HPP
