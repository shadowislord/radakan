#include "obstacle.hpp"

using namespace std;

//  Constructor
Obstacle::
	Obstacle (string new_name, D3 new_position, D3 new_size):
	Location::
	Location (new_name, new_position, new_size)
{
	assert (Location::is_initialized ());
	
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
	is_initialized
	()
	const
{
	return Location::is_initialized ();
}

//	virtual
void
	Obstacle::
	draw
	(SHR * shr)
	const
{
	assert (is_initialized ());

}
