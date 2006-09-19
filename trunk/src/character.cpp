#include "character.hpp"

using namespace std;

//  constructor
Character ::
	Character
		(Ogre :: Entity * new_ogre_entity,
		Ogre :: SceneNode * new_node) :
	Object
		((new_ogre_entity == NULL) ?
		"[ERROR: new_ogre_entity is NULL]" :
		new_ogre_entity -> getName ()),
	Container
		(true,
		true,
		true,
		80,
		65,
		Ogre :: Vector3 (100, 100, 200),
		new_ogre_entity,
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
	assert (Object :: is_initialized (* this + " -> ~Character ()"));
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
	add (Object * sub_tree)
{
	assert (is_initialized ());
	assert (sub_tree != NULL);
	assert (sub_tree -> is_initialized ());
	assert (sub_tree -> is_type <Entity> ());

	if ((weapon == NULL) && sub_tree -> is_type <Weapon> ())
	{
		Container :: add (sub_tree);
		weapon = sub_tree -> to_type <Weapon> ();
		return true;
	}
	else if ((backpack == NULL) && sub_tree -> is_type <Container> ())
	{
		Container :: add (sub_tree);
		backpack = sub_tree -> to_type <Container> ();
		return true;
	}
	else if (backpack != NULL)
	{
		return backpack -> add (sub_tree);
	}
	else
	{
		return false;
	}
}

//	virtual
bool Character ::
	move_to (Object * sub_tree, Object * other_tree)
{
	assert (is_initialized ());
	assert (sub_tree != NULL);
	assert (other_tree != NULL);
	assert (sub_tree -> is_initialized ());
	assert (other_tree -> is_initialized ());
	assert (contains (sub_tree, false));

	if (weapon == sub_tree)
	{
		weapon = NULL;
	}
	else if (backpack == sub_tree)
	{
		backpack = NULL;
	}
	bool result = Container :: move_to (sub_tree, other_tree);

	if (backpack != NULL)
	{
		result = result || backpack -> move_to (sub_tree, other_tree);
	}

	return result;
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
