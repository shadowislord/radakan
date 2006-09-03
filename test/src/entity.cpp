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
		Ogre :: Vector3 new_position,
		Ogre :: Entity * new_ogre_entity,
		Ogre :: SceneNode * new_node):
	Object (new_name),
	movable (new_movable),
	solid (new_solid),
	volume (new_volume),
	weight (new_weight)
{
	assert (Object :: is_initialized ());

	position = new_position;

	node = new_node;
	ogre_entity = new_ogre_entity;

	if (new_ogre_entity == NULL)
	{
		assert (new_node == NULL);
		//	this will be disabled later
	}
	else
	{
		assert (new_node != NULL);
		
		node->attachObject (ogre_entity);
		node->setPosition (new_position);
	}
	
	assert (is_initialized ());
}

//  destructor
Entity ::
	~Entity ()
{
	assert (Object :: is_initialized (* this + "->Entity :: ~Entity ()"));

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
	assert (is_initialized ());
	
	return weight;
}
