#include "container.hpp"

using namespace std;
using namespace tsl;

//  constructor
Container ::
	Container
		(bool new_movable,
		bool new_solid,
		bool new_visible,
		float new_volume,
		float new_weight,
		btVector3 new_position,
		Ogre :: SceneNode & new_node) :
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

//	static
string Container ::
	get_class_name ()
{
	return "Container";
}

//	virtual
float Container ::
	get_total_weight ()
	const
{
	assert (is_initialized ());
	
	float total_weight = weight;

	for (Entity * i = get_child (); i != NULL; i = get_another_child ())
	{
		assert (i -> is_type <Container> ());
		total_weight += i -> to_type <Container> () . get_total_weight ();
	}

	return total_weight;
}

//	virtual
bool Container ::
	add (Entity & entity)
{
	assert (is_initialized ());
	assert (entity . is_initialized ());

	bool result = Set <Entity> :: add (entity);
	entity . node -> setVisible (false);
	return result;
}

//	virtual
bool Container ::
	move_to (Entity & entity, Set <Entity> & other_set)
{
	assert (is_initialized ());
	assert (entity . is_initialized ());
	assert (other_set . is_initialized ());
	assert (contains (entity, false));

	//	Enitities inside a container are not visible. But when an enitity
	//	is moved to a plain Set <Enitity>, is should be visible.
	entity . node -> setVisible (true);
	bool result = Set <Entity> :: move_to (entity, other_set);
	if (! result)
	{
		entity . node -> setVisible (false);
	}
	return result;
}
