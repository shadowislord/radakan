#include "entity.hpp"

using namespace std;

//  constructor
Entity ::
	Entity
		(string new_name,
		bool new_movable,
		bool new_solid,
		float new_volume,
		float new_weight,
		Ogre :: Vector3 new_position):
	Object (new_name),
	movable (new_movable),
	solid (new_solid),
	volume (new_volume),
	weight (new_weight)
{
	assert (Object :: is_initialized ());

	position = new_position;

	assert (is_initialized ());
}

//  destructor
Entity ::
	~Entity ()
{
	assert (is_initialized ());
}

//	virtual
bool Entity ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
float Entity ::
	get_total_weight ()
	const
{
	return weight;
}
