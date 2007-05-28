#include "conversation_message.hpp"
#include "log.hpp"
#include "npc.hpp"
#include "player_character.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Items;

//	static
const string Player_Character ::
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

	bool check = back . add (Container :: create ("Backbpack", "bar.mesh", Ogre :: Vector3 (0.3, 0.5, 0.2), 3));
	assert (check);

	check = hands . add (Weapon :: create ("Sword", "bar.mesh", Ogre :: Vector3 (0.1, 0.2, 0.3), 4, 5, 6, 7, 8, 9, 10));
	assert (check);

	assert (is_initialized ());
}

//  destructor
Player_Character ::
	~Player_Character ()
{
	Engines :: Log :: trace (me, Player_Character :: get_class_name (), "~");
	assert (is_initialized ());

	forget_dependencies ();
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
	call (const Object & message)
{
	assert (is_initialized ());
	assert (message . is_type <Messages :: Conversation_Message> ());

	//	I show the message in the log, to let the player know.
	Engines :: Log :: show (message . to_type <Messages :: Conversation_Message> () . from + ": " + message);
}

//	static
Item & Player_Character ::
	create
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass
	)
{
	Item * temp =
		new Player_Character
		(
			new_name,
			new_mesh_name,
			new_size,
			new_mass
		);

	return * temp;
}
