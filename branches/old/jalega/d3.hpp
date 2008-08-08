#ifndef D3_HPP
#define D3_HPP

#include "object.hpp"

using namespace std;

/// This represent a vector in a two dimensional plane.
class D3:
	public Object
{
    public:
		D3 ();
		D3 (float r);
		D3 (float new_x, float new_y, float new_z);
		D3 (string new_name, D3 b);
		virtual ~D3 ();
		virtual bool is_initialized () const;
		
		float length () const;
		float distance (D3 b) const;
		D3 operator- () const;
		D3 operator* (float a) const;
		D3 operator/ (float a) const;
		D3 operator+ (D3 b) const;
		D3 operator- (D3 b) const;
		void operator= (D3 b);
		void operator+= (D3 b);
		bool operator== (D3 b) const;
		bool operator< (D3 b) const;
		bool operator> (D3 b) const;
		float get_x () const;
		float get_y () const;
		float get_z () const;

	protected:
		float x , y, z;
};

#endif
