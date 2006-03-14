#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "object.hpp"

using namespace std;

class Obstacle:
	public Object
{
	public:
		Obstacle (string new_name);							//	Constructor
		virtual ~Obstacle ();								//	Destructor
		virtual bool is_initialized () const;
};

#endif
