#include "obstacle.hpp"

using namespace std;

//  Constructor
Obstacle::
	Obstacle
		(string new_name,
		Vector3 new_size,
		bool new_movable,
		float new_volume,
		float new_weight):
	Sl_Entity (new_name, new_size),
	movable (new_movable),
	volume (new_volume),
	weight (new_weight)
{
	assert (Sl_Entity::is_initialized ());

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
	return Object::is_initialized () && (0 < size.squaredLength());
}
