// SHPoint.h: interface for the SHPoint class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SH_POINT_HPP
#define SH_POINT_HPP

class SH_Point
{
	public:
		SH_Point (float x, float y);
		virtual ~SH_Point();
		float getX ();
		float getY ();
		void setX (float x);
		void setY (float y);
	private:
		float x;
		float y;
};

#endif // SH_POINT_HPP
