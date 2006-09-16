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
		Ogre :: SceneNode * new_node):
	Object (
		(new_ogre_entity == NULL) ?
		"[ERROR: new_ogre_entity is NULL]" :
		new_ogre_entity->getName ()),
	movable (new_movable),
	solid (new_solid),
	visible (new_visible),
	volume (new_volume),
	weight (new_weight)
{
	assert (Object :: is_initialized ());
	assert (new_ogre_entity != NULL);
	assert (new_node != NULL);

	container_parent = NULL;
	
	position = new_position;

	node = new_node;
	ogre_entity = new_ogre_entity;

	ogre_entity->setVisible (visible);
	node->attachObject (ogre_entity);
	node->setPosition (new_position);

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

void Entity ::
	remove_from_container (Entity * old_container_parent)
{
	assert (is_initialized ());
	assert (is_in_container (old_container_parent));

	container_parent = NULL;
}

void Entity ::
	put_in_container (Entity * new_container_parent)
{
	assert (is_initialized ());
	assert (container_parent == NULL);
	assert (new_container_parent != NULL);

	container_parent = new_container_parent;

	//	only items directely in the sector are dispayed, when visible:
	ogre_entity->setVisible (visible && container_parent->is_in_container (NULL));
}

bool Entity ::
	is_in_container (const Entity * container)
	const
{
	assert (is_initialized ());
	
	return (container_parent == container);
}
