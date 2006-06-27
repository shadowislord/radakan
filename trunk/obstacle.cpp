#include "obstacle.hpp"

using namespace std;

//  Constructor
Obstacle::
	Obstacle (string new_name/*, Vector3 new_position, D3 new_size*/):
	Object::
	Object (new_name)
{
	assert (Object::is_initialized ());


	
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
	return Object::is_initialized ();
}
