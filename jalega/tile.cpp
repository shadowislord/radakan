#include "tile.hpp"

using namespace std;

//  Constructor
Tile::
	Tile ():
	Object::
	Object ("anonnymous_tile")
{
	obstacle = NULL;
	assert (is_initialized ());
}

//  Constructor
Tile::
	Tile (string new_name):
	Object::
	Object (new_name)
{
	obstacle = NULL;
	assert (is_initialized ());
}

//  Destructor
Tile::
	~Tile ()
{
	assert (is_initialized ());

	if (has_obstacle ())
	{
		delete obstacle;
	}
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

	return (obstacle != NULL);
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

//	virtual
void
	Tile::
	draw
	(unsigned int left, unsigned int top)
	const
{
	shr->render_quad (new D3 (left, top, 0), 20, 20, 0, 0, 0);
}
