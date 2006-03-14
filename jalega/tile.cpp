#include "tile.hpp"

using namespace std;

//  Constructor
Tile::
	Tile ():
	Object::
	Object ("unnamed_tile")
{
	assert (is_initialized ());
}

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
