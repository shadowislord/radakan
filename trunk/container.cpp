#include "container.hpp"

using namespace std;

//  constructor
Container ::
	Container
		(string new_name,
		bool new_movable,
		float new_volume,
		float new_weight,
		Ogre :: Vector3 new_position,
		Ogre :: Entity * new_ogre_entity,
		Ogre :: SceneNode * new_node) :
	Entity
		(new_name,
		new_movable,
		true,
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

	//	second means we're interested in if it worked or not.
	//	first would give a iterator to the item
	return items.insert (item).second;
}

//	virtual
bool Container ::
	remove (Entity * item)
{
	assert (is_initialized ());

	return (0 < items.erase (item));
}

//	virtual
bool Container ::
	contains (Entity * item)
	const
{
	assert (is_initialized ());

	return (items.find (item) != items.end ());
}
