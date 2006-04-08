#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "shr.hpp"

using namespace std;

class Obstacle:
	public Object
{
	public:
		Obstacle (string new_name);							//	Constructor
		virtual ~Obstacle ();								//	Destructor
		virtual bool is_initialized () const;
		virtual void draw (unsigned int left, unsigned int top) const;
};

#endif
