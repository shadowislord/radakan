#include "obstacle.hpp"

using namespace std;

//  constructor
Tsl_Entity::
	Tsl_Entity (string new_name):
	//Object::
	Object (new_name)
{
	assert (Object::is_initialized ());


	assert (is_initialized ());
}

//  destructor
Tsl_Entity::
	~Tsl_Entity ()
{
	assert (is_initialized ());
}

//	virtual
bool
	Tsl_Entity::
	is_initialized
	()
	const
{
	return Object::is_initialized ();
}
