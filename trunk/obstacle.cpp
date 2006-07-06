#include "obstacle.hpp"

using namespace std;

//  Constructor
Obstacle::
	Obstacle (string new_name, Vector3 new_size):
	//Tsl_Entity::
	Tsl_Entity (new_name)
{
	assert (Tsl_Entity::is_initialized ());

	size = new_size;

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
	return (0 < size.squaredLength()) && Object::is_initialized ();
}
