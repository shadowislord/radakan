#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "obstacle.hpp"

using namespace std;

class Character:
	public Obstacle
{
	public:
		Character (string new_name, D3 new_position, D3 new_size);
		virtual ~Character ();
		virtual void draw (SHR * shr) const;
};

#endif
