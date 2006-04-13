#ifndef TILE_HPP
#define TILE_HPP

#include "obstacle.hpp"

using namespace std;

class Tile:
	public Object
{
	public:
		Tile (string new_name);							//	constructor
		virtual ~Tile ();								//	destructor
		virtual void draw (SHR * shr, int left, int bottom) const;
};

#endif
