#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "obstacle.hpp"

using namespace std;

class Character:
	public Obstacle
{
	public:
		Character (string new_name, D3 * new_position);		//	constructor
		virtual ~Character ();								//	destructor
		virtual void draw (SHR * shr, int left, int bottom) const;
};

#endif
