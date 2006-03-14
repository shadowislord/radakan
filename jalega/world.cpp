#include "world.hpp"

using namespace std;
using namespace boost;

//  Constructor
World::
	World (string new_name, int new_width, int new_height):
	Object::
	Object (new_name)
{
	tiles = new multi_array <Tile, 2> (extents[new_width][new_height]);

	assert (is_initialized ());
}

//  Destructor
World::
	~World ()
{

}

//	virtual
bool
	World::
	is_initialized ()
	const
{
	return Object::is_initialized ();
}

void
	World::
	set_tile (Tile * new_tile, int x, int y)
{
	assert (is_initialized ());

	tiles[x][y] = new_tile;
}
