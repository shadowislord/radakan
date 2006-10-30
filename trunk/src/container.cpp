#include "container.hpp"

using namespace std;
using namespace sl;

//  constructor
Container ::
	Container
		(Ogre :: SceneNode * new_node) :
	Object (get_name (new_node)),
	Entity
		(false,
		false,
		false,
		0,
		0,
		Ogre :: Vector3 (0, 0, 0),
		new_node),
	Set <Entity> (get_name (new_node))
{
	assert (Set <Entity> :: is_initialized ());
	
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
		Ogre :: SceneNode * new_node) :
	Object (get_name (new_node)),
	Entity
		(new_movable,
		new_solid,
		new_visible,
		new_volume,
		new_weight,
		new_position,
		new_node),
	Set <Entity> (get_name (new_node))
{
	assert (Set <Entity> :: is_initialized ());
	
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
	return Set <Entity> :: is_initialized ();
}

//	virtual
float Container ::
	get_total_weight ()
	const
{
	assert (is_initialized ());
	
	float total_weight = weight;

	for (set <Entity *> :: const_iterator i = children . begin ();
													i != children . end (); i ++)
	{
		assert ((* i) -> is_type <Container> ());
		total_weight += (* i) -> to_type <Container> () -> get_total_weight ();
	}

	return total_weight;
}

//	virtual
bool Container ::
	add (Entity * entity)
{
	assert (is_initialized ());
	assert (entity != NULL);
	assert (entity -> is_initialized ());

	return Set <Entity> :: add (entity);
}
