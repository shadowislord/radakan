#ifndef D3_HPP
#define D3_HPP

#include "object.hpp"

using namespace std;

/// This represent a vector in a two dimensional plane.
class D3:
	public Object
{
    public:
		D3 (float new_x , float new_y, float new_z);
		D3 (const D3 &b);
		virtual ~D3 ();
		
		double length () const;
		double distance (const D3 &b) const;
		D3 half () const;
		D3 operator- () const;
		D3 operator* (float a) const;
		D3 operator/ (float a) const;
		D3 operator+ (const D3 &b);
		D3 operator- (const D3 &b);
		void operator= (const D3 &b);
		void operator+= (const D3 &b);
		bool operator== (const D3 &b);
		
		float x , y, z;
};

#endif
