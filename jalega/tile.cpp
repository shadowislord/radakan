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

//  Destructor
Tile::
	~Tile ()
{
	assert (is_initialized ());
	
}

//	virtual
bool
	Tile::
	is_initialized ()
	const
{
	if (obstacle == NULL)					//	don't use has_object ()
	{
		return Object::is_initialized ();
	}
	return Object::is_initialized () && obstacle->is_initialized ();
}

bool
	Tile::
	has_obstacle ()
{
	assert (is_initialized ());

	return (obstacle == NULL);
}

void
	Tile::
	set_obstacle (Obstacle * new_obstacle)	//	remove the old one first
{
	assert (! has_obstacle ());

	obstacle = new_obstacle;
}

void
	Tile::
	remove_obstacle ()						//	make sure there is one first
{
	assert (has_obstacle ());
	
	obstacle = NULL;
}

Obstacle *
	Tile::
	get_obstacle ()							//	make sure there is one first
{
	assert (has_obstacle ());
	
	return obstacle;
}
