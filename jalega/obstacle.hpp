#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "shr.hpp"

using namespace std;

class Obstacle:
	public Object
{
	public:
		Obstacle (string new_name, D3 * new_position);		//	constructor
		virtual ~Obstacle ();								//	destructor
		virtual bool is_initialized () const;
		virtual void draw (SHR * hsr, int left, int bottom) const;
		void move (D3 * new_position);
		D3 * position;
};

#endif
