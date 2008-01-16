#include "engines/log.hpp"
#include "items/characters/player_character.hpp"
#include "items/weapon.hpp"
#include "messages/conversation_message.hpp"
#include "strategies/behaviors/player.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;
using namespace Radakan :: Items :: Characters;

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
		float new_mass,
		Ogre :: Vector3 new_size,
		const Reference <Mesh_Data> new_mesh_data
	) :
	Object (new_name),
	Character
	(
		"Doesn't matter.",
		new_mass,
		new_size,
		new_mesh_data
	)
{
	Engines :: Log :: trace (me, Player_Character :: get_class_name (), "", new_name, to_string (new_mass), to_string (new_size), new_mesh_data . get_name ());

	set_active_state (Reference <Strategies :: Behaviors :: Behavior>
		(new Strategies :: Behaviors :: Player (Reference <Character> (this))));

	bool check = back -> add (Reference <Container_Item <Item> > (new Container_Item <Item> ("Backpack", 3, Ogre :: Vector3 (0.3, 0.5, 0.2), Reference <Mesh_Data> ())));
	assert (check);

	check = right_hand -> add (Reference <Item> (new Weapon ("Sword", 4, Ogre :: Vector3 (0.1, 0.2, 0.3), Reference <Mesh_Data> (), 5, 6, 7, 8, 9, 10)));
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
		Engines :: Log :: show (me . get_name (true) + " died!");
		
		return;
	}
	else
	{
		//	The NPCs message are showed in the log, to let the player know.
		Engines :: Log :: show
			(message -> from . get_name (true) + ": " + message . get_name (true));
	}
}
