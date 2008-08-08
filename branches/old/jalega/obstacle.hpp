#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "location.hpp"

using namespace std;

class Obstacle:
	public Location
{
	public:
		Obstacle (string new_name, D3 new_position, D3 new_size);
		virtual ~Obstacle ();
		virtual bool is_initialized () const;
		virtual void draw (SHR * hsr) const;
};

#endif
