#include "entity.hpp"

using namespace std;
using namespace sl;

//  constructor
Entity ::
	Entity
		(bool new_movable,
		bool new_solid,
		bool new_visible,
		float new_volume,
		float new_weight,
		Ogre :: Vector3 new_position,
		Ogre :: SceneNode * new_node) :
	Object (get_name (new_node)),
	movable (new_movable),
	solid (new_solid),
	volume (new_volume),
	weight (new_weight)
{
	assert (Object :: is_initialized ());
	assert (new_node != NULL);

	node = new_node;
	node -> setVisible (new_visible);
	node -> setPosition (new_position);

	assert (is_initialized ());
}

//  destructor
Entity ::
	~Entity ()
{
	trace () << "~Entity ()" << endl;
	assert (Object :: is_initialized ());
}

//	virtual
bool Entity ::
	is_initialized ()
	const
{
	return Object :: is_initialized () && (0 <= volume) && (0 <= weight) && (node != NULL);
}

//	virtual
float Entity ::
	get_total_weight ()
	const
{
	trace () << "get_total_weight ()" << endl;
	assert (Object :: is_initialized ());
	
	return weight;
}

string sl :: get_name (Ogre :: SceneNode * node)
{
	assert (node != NULL);
	assert (node -> numAttachedObjects () == 1);
	Ogre :: MovableObject * movable_object = node -> getAttachedObject (0);
	assert (movable_object != NULL);

	return movable_object -> getName ();
}
