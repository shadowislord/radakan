#include "container.hpp"

using namespace std;

//  constructor
Container ::
	Container
		(Ogre :: Entity * new_ogre_entity,
		Ogre :: SceneNode * new_node):
	Object
		((new_ogre_entity == NULL) ?
		"[ERROR: new_ogre_entity is NULL]" :
		new_ogre_entity->getName ()),
	Entity
		(false,
		false,
		false,
		0,
		0,
		Ogre :: Vector3 (0, 0, 0),
		new_ogre_entity,
		new_node),
	Tree
		((new_ogre_entity == NULL) ?
		"[ERROR: new_ogre_entity is NULL]" :
		new_ogre_entity->getName ())
{
	assert (Entity :: is_initialized ());
	assert (Tree :: is_initialized ());
	
	assert (is_initialized ());
}

//  constructor
Container ::
	Container
		(bool new_movable,
		bool new_solid,
		bool new_visible,
		float new_volume,
		float new_weight,
		Ogre :: Vector3 new_position,
		Ogre :: Entity * new_ogre_entity,
		Ogre :: SceneNode * new_node):
	Object
		((new_ogre_entity == NULL) ?
		"[ERROR: new_ogre_entity is NULL]" :
		new_ogre_entity->getName ()),
	Entity
		(new_movable,
		new_solid,
		new_visible,
		new_volume,
		new_weight,
		new_position,
		new_ogre_entity,
		new_node),
	Tree
		((new_ogre_entity == NULL) ?
		"[ERROR: new_ogre_entity is NULL]" :
		new_ogre_entity->getName ())
{
	assert (Entity :: is_initialized ());
	assert (Tree :: is_initialized ());
	
	assert (is_initialized ());
}

//  destructor
Container ::
	~Container ()
{
	assert (is_initialized ());

}

//	virtual
bool Container ::
	is_initialized ()
	const
{
	return Entity :: is_initialized () && Tree :: is_initialized ();
}

//	virtual
float Container ::
	get_total_weight ()
	const
{
	assert (is_initialized ());
	
	float total_weight = weight;

	for (set <Object *> :: const_iterator i = children.begin ();
													i != children.end (); i ++)
	{
		assert ((* i)->is_type <Container> ());
		total_weight += (* i)->to_type <Container> ()->get_total_weight ();
	}

	return total_weight;
}

//	returns subtree, iff succes
//	virtual
bool Container ::
	add (Object * sub_tree)
{
	assert (is_initialized ());
	assert (sub_tree != NULL);
	assert (sub_tree->is_initialized ());
	assert (sub_tree->is_type <Entity> ());
	
	return Tree :: add (sub_tree);
}
