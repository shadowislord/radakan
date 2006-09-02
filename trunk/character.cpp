#include "character.hpp"

using namespace std;

//  constructor
Character ::
	Character (string new_name) :
	Container
		(new_name,
		true,
		80,
		65,
		Ogre :: Vector3 (0, 1, 2))
{
	assert (Container :: is_initialized ());

	dead = false;

	backpack = new Container
		(* this + "'s backpack",
		true,
		30,
		3,
		this->position);

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
	else
	{
		return backpack->add (item);
	}
}

//	virtual
bool Character ::
	remove (Entity * item)
{
	assert (is_initialized ());

	if (weapon == item)
	{
		weapon = NULL;
	}
	return (Container :: remove (item) || backpack->remove (item));
}

//	virtual
bool Character ::
	contains (Entity * item)
	const
{
	assert (is_initialized ());

	return (Container :: contains (item) || backpack->contains (item));
}
