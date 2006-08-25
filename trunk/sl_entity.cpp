#include "obstacle.hpp"

using namespace std;

//  constructor
Sl_Entity::
	Sl_Entity (string new_name):
	//	Object::
	Object (new_name)
{
	assert (Object::is_initialized ());


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
	return Object::is_initialized ();
}
