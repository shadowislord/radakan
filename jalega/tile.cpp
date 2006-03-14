#include "tile.hpp"

using namespace std;

//  Constructor
Tile::
	Tile (string new_name):
	Object::
	Object (new_name)
{
	assert (is_initialized ());
}

//  Destructor
Tile::
	~Tile ()
{

}

//	virtual
bool
	Tile::
	is_initialized ()
	const
{
	return Object::is_initialized ();
}
