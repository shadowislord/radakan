#ifndef TILE_HPP
#define TILE_HPP

#include "location.hpp"

using namespace std;

class Tile:
	public Location
{
	public:
		Tile (string new_name, D3 new_position, D3 new_size);
		virtual ~Tile ();
};

#endif
