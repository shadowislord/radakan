#include "character.hpp"

using namespace std;
using namespace sl;

//  constructor
Character ::
	Character (Ogre :: SceneNode * new_node) :
	Object (get_name (new_node)),
	Container
		(true,
		true,
		true,
		80,
		65,
		Ogre :: Vector3 (100, 0, 200),
		new_node)
{
	assert (Entity :: is_initialized ());

	dead = false;

	backpack = NULL;
	weapon = NULL;

	assert (is_initialized ());
}

//  destructor
Character ::
	~Character ()
{
	assert (Object :: is_initialized ("~Character ()"));
}

//	virtual
bool Character ::
	is_initialized ()
	const
{
	return Entity :: is_initialized () && (backpack == NULL || backpack -> is_initialized ()) && (weapon == NULL || weapon -> is_initialized ());
}

//	virtual
float Character ::
	get_total_weight ()
	const
{
	assert (is_initialized ());

	float weight = Entity :: get_total_weight ();

	if (backpack != NULL)
	{
		weight += backpack -> get_total_weight ();
	}
	
	return weight;
}

//	virtual
bool Character ::
	add (Entity * entity)
{
	assert (is_initialized ());
	assert (entity != NULL);
	assert (entity -> is_initialized ());

	if ((weapon == NULL) && entity -> is_type <Weapon> ())
	{
		Container :: add (entity);
		weapon = entity -> to_type <Weapon> ();
		return true;
	}
	else if ((backpack == NULL) && entity -> is_type <Container> ())
	{
		Container :: add (entity);
		backpack = entity -> to_type <Container> ();
		return true;
	}
	else
	{
		return false;
	}
}

//	virtual
bool Character ::
	move_to (Entity * entity, Set <Entity> * other_set)
{
	assert (is_initialized ());
	assert (entity != NULL);
	assert (other_set != NULL);
	assert (entity -> is_initialized ());
	assert (other_set -> is_initialized ());
	assert (contains (entity, false));

	bool result = Container :: move_to (entity, other_set);

	if (result)
	{
		if (weapon == entity)
		{
			weapon = NULL;
		}
		else if (backpack == entity)
		{
			backpack = NULL;
		}
	}
	
	assert (result == other_set -> contains (entity, false));

	return result;
}

void Character ::
	run (float distance)
{
	assert (is_initialized ());

	node -> translate (node -> getOrientation () * Ogre :: Vector3 (0, 0, - distance));
}

void Character ::
	turn (float radian_angle)
{
	assert (is_initialized ());

	node -> yaw (Ogre :: Radian (radian_angle));
}

string Character ::
	die ()
{
	assert (is_initialized ());

	dead = true;

	return * this + " died!";
}

bool Character ::
	is_dead ()
	const
{
	assert (is_initialized ());

	return dead;
}

bool Character ::
	has_weapon ()
	const
{
	assert (is_initialized ());

	return (weapon != NULL);
}

Weapon * Character ::
	get_weapon ()
	const
{
	assert (is_initialized ());
	assert (has_weapon ());

	return weapon;
}
