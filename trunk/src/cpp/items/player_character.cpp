#include "engines/log.hpp"
#include "items/player_character.hpp"
#include "items/weapon.hpp"
#include "messages/conversation_message.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;

//	static
string Player_Character ::
	get_class_name ()
{
	return "Player_Character";
}

//  constructor
Player_Character ::
	Player_Character
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass
	) :
	Object (new_name),
	Character
	(
		new_mesh_name,
		new_size,
		new_mass
	),
	dead (false)
{
	Engines :: Log :: trace (me, Player_Character :: get_class_name (), "", new_name, new_mesh_name, to_string (new_size), to_string (new_mass));

	bool check = back -> add (Reference <Container_Item <Item> > (new Container_Item <Item> ("Backpack", "bar.mesh", Ogre :: Vector3 (0.3, 0.5, 0.2), 3)));
	assert (check);

	check = right_hand -> add (Reference <Item> (new Weapon ("Sword", "bar.mesh", Ogre :: Vector3 (0.1, 0.2, 0.3), 4, 5, 6, 7, 8, 9, 10)));
	assert (check);

	assert (is_initialized ());
}

//  destructor
Player_Character ::
	~Player_Character ()
{
	Engines :: Log :: trace (me, Player_Character :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Player_Character ::
	is_initialized ()
	const
{
	return Character :: is_initialized ();
}

//	virtual
bool Player_Character ::
	is_dead () const
{
	return dead;
}

//	virtual
void Player_Character ::
	die ()
{
	assert (is_initialized ());
	dead = true;

	Engines :: Log :: show ("Your character died!");
}

//	virtual
void Player_Character ::
	call (const Reference <Messages :: Message <Character> > & message)
{
	assert (is_initialized ());

	if (message == Messages :: Message <Character> :: update)
	{
		return;
	}
	else if (message == Messages :: Message <Character> :: terminate)
	{
		die ();
	}
	else
	{
		//	I show the NPC's message in the log, to let the player know.
		Engines :: Log :: show (message -> from . get_name () + ": " + message . get_name ());
	}
}
