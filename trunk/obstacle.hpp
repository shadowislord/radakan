#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "object.hpp"

using namespace std;

class Obstacle:
	public Object
{
	public:
		Obstacle (string new_name, Vector3 new_position/*, D3 new_size*/);
		virtual ~Obstacle ();
		virtual bool is_initialized () const;
};

#endif
