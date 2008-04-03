#include "tile.hpp"

using namespace std;

//  constructor
Tile::
	Tile (string new_name, D3 new_position, D3 new_size):
	Location::
	Location (new_name, new_position, new_size)
{
	assert (Location::is_initialized ());
	
	assert (is_initialized ());
}

//  destructor
Tile::
	~Tile ()
{
	assert (is_initialized ());
}
