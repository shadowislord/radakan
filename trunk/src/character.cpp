#include "character.hpp"

using namespace std;

//  constructor
Character ::
	Character
		(Ogre :: Entity * new_ogre_entity,
		Ogre :: SceneNode * new_node) :
	Container
		(true,
		true,
		80,
		65,
		Ogre :: Vector3 (100, 100, 200),
		new_ogre_entity,
		new_node)
{
	assert (Container :: is_initialized ());

	dead = false;

	backpack = NULL;
	weapon = NULL;
	
	assert (is_initialized ());
}

//  destructor
Character ::
	~Character ()
{
	assert (is_initialized ());

	delete backpack;
}

//	virtual
bool Character ::
	is_initialized ()
	const
{
	return Container :: is_initialized ();
}

//	virtual
float Character ::
	get_total_weight ()
	const
{
	assert (is_initialized ());
	
	return Container :: get_total_weight () + backpack->get_total_weight ();
}

//	virtual
bool Character ::
	add (Entity * item)
{
	assert (is_initialized ());

	if ((weapon == NULL) && item->is_type <Weapon> ())
	{
		Container :: add (item);
		weapon = item->to_type <Weapon> ();
		return true;
	}
	else if ((backpack == NULL) && item->is_type <Container> ())
	{
		Container :: add (item);
		backpack = item->to_type <Container> ();
		return true;
	}
	else if (backpack != NULL)
	{
		return backpack->add (item);
	}
	else
	{
		return false;
	}
}

//	virtual
bool Character ::
	move_to (Entity * item, Container * new_container)
{
	assert (is_initialized ());
	assert (contains (item));

	if (weapon == item)
	{
		weapon = NULL;
		
	}
	return (Container :: move_to (item, new_container)
								|| backpack->move_to (item, new_container));
}

//	virtual
bool Character ::
	contains (Entity * item)
	const
{
	assert (is_initialized ());

	return (Container :: contains (item) || backpack->contains (item));
}

void Character ::
	die ()
{
	assert (is_initialized ());

	dead = true;

	debug () << *this << " died!" << endl;
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
