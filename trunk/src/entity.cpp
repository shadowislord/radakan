#include "entity.hpp"

using namespace std;

//  constructor
Entity ::
	Entity
		(bool new_movable,
		bool new_solid,
		bool new_visible,
		float new_volume,
		float new_weight,
		Ogre :: Vector3 new_position,
		Ogre :: Entity * new_ogre_entity,
		Ogre :: SceneNode * new_node) :
	Object (
		(new_ogre_entity == NULL) ?
		"[ERROR: new_ogre_entity is NULL]" :
		new_ogre_entity -> getName ()),
	movable (new_movable),
	solid (new_solid),
	visible (new_visible),
	volume (new_volume),
	weight (new_weight)
{
	assert (Object :: is_initialized ());
	assert (new_ogre_entity != NULL);
	assert (new_node != NULL);

	position = new_position;

	node = new_node;
	ogre_entity = new_ogre_entity;

	ogre_entity -> setVisible (visible);
	node -> attachObject (ogre_entity);
	node -> setPosition (new_position);

	assert (is_initialized ());
}

//  destructor
Entity ::
	~Entity ()
{
	assert (Object :: is_initialized (* this + " -> ~Entity ()"));
}

//	virtual
bool Entity ::
	is_initialized ()
	const
{
	return Object :: is_initialized () && (0 <= volume) && (0 <= weight) && (ogre_entity != NULL) && (node != NULL);
}

//	virtual
float Entity ::
	get_total_weight ()
	const
{
	assert (is_initialized ());
	
	return weight;
}

/* !!!	//	only items directely in the sector are dispayed, when visible:
	ogre_entity -> setVisible (visible && tree_parent -> is_in_tree (NULL));*/
