#include "obstacle.hpp"

using namespace std;

//  constructor
Sl_Entity::
	Sl_Entity (string new_name, Vector3 new_size):
	Object (new_name),
	size (new_size)
{
	assert (Object::is_initialized ());

	position = Vector3 (1, 1, 1);

	assert (is_initialized ());
}

//  destructor
Sl_Entity::
	~Sl_Entity ()
{
	assert (is_initialized ());
}

//	virtual
bool
	Sl_Entity::
	is_initialized
	()
	const
{
	return Object::is_initialized () && (0 < size.squaredLength ());
}
