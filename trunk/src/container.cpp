#include "container.hpp"

using namespace std;

//  constructor
Container ::
	Container
		(bool new_movable,
		bool new_visible,
		float new_volume,
		float new_weight,
		Ogre :: Vector3 new_position,
		Ogre :: Entity * new_ogre_entity,
		Ogre :: SceneNode * new_node) :
	Entity
		(new_movable,
		true,
		new_visible,
		new_volume,
		new_weight,
		new_position,
		new_ogre_entity,
		new_node)
{
	assert (Entity :: is_initialized ());
	
	assert (is_initialized ());
}

//  destructor
Container ::
	~Container ()
{
	assert (is_initialized ());

	for (set <Entity *> :: const_iterator i = items.begin (); i != items.end (); i ++)
	{
		delete (* i);
	}
}

//	virtual
bool Container ::
	is_initialized ()
	const
{
	return Entity :: is_initialized ();
}

//	virtual
float Container ::
	get_total_weight ()
	const
{
	assert (is_initialized ());
	
	float total_weight = weight;

	for (set <Entity *> :: const_iterator i = items.begin (); i != items.end (); i ++)
	{
		total_weight += (* i)->get_total_weight ();
	}

	return total_weight;
}

//	virtual
bool Container ::
	add (Entity * item)
{
	assert (is_initialized ());
	assert (item->is_in_container (NULL));

	debug () << * item << " added to " << * this << endl;
	item->put_in_container (this);
	
	//	second means we're interested in if it worked or not.
	//	first would give a iterator to the item
	return items.insert (item).second;
}

//	virtual
bool Container ::
	move_to (Entity * item, Container * new_container)
{
	assert (is_initialized ());
	assert (contains (item));

	item->remove_from_container (this);
	if (new_container->add (item))
	{
		items.erase (item);
		return true;
	}
	item->put_in_container (this);
	return false;
}

//	virtual
bool Container ::
	contains (Entity * item)
	const
{
	assert (is_initialized ());

	bool result = (items.find (item) != items.end ());
	
	assert (result == (item->is_in_container (this)));
	return result;
}
