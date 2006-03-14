#include "tile.hpp"

using namespace std;

//  Constructor
Obstacle::
	Obstacle (string new_name):
	Object::
	Object (new_name)
{
	assert (is_initialized ());
}

//  Destructor
Obstacle::
	~Obstacle ()
{
	assert (is_initialized ());
}

//	virtual
bool
	Obstacle::
	is_initialized ()
	const
{
	return Object::is_initialized ();
}
