#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "obstacle.hpp"

using namespace std;

class Character:
	public Obstacle
{
	public:
		Character (string new_name, D3 * new_position, float new_radius);
		virtual ~Character ();
		virtual void draw (SHR * shr, float left, float bottom) const;
};

#endif
