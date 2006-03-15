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

Tile *
	World::
	get_tile (int x, int y)
{
	assert (is_initialized ());

	return & tiles[x][y][0];	//	I've no idea why the [0] needs to be added.
}
