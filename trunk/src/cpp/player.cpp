#include "log.hpp"
#include "player.hpp"

using namespace std;
using namespace TSL;

//	static
const string Player ::
	get_class_name ()
{
	return "Player";
}

//  constructor
Player ::
	Player
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
	camera_distance (0.72),
	dead (false)
{
	Log :: trace <Player> (me, "", new_name, new_mesh_name, to_string (new_size), to_string (new_mass));
	assert (Character :: is_initialized ());

	bool check = back . add (Container :: create ("Backbpack", "bar.mesh", Ogre :: Vector3 (0.3, 0.5, 0.2), 3));
	assert (check);

	check = hands . add (Weapon :: create ("Sword", "bar.mesh", Ogre :: Vector3 (0.1, 0.2, 0.3), 4, 5, 6, 7, 8, 9, 10));
	assert (check);

	assert (is_initialized ());
}

//  destructor
Player ::
	~Player ()
{
	Log :: trace <Player> (me, "");
	assert (Object :: is_initialized ());
}

//	virtual
bool Player ::
	is_initialized ()
	const
{
	return Character :: is_initialized ();
}

//	virtual
bool Player ::
	is_dead () const
{
	return dead;
}

//	virtual
void Player ::
	die ()
{
	assert (is_initialized ());
	dead = true;

	Log :: show ("Your character died!");
}

//	static
Item & Player ::
	create
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass
	)
{
	Item * temp =
		new Player
		(
			new_name,
			new_mesh_name,
			new_size,
			new_mass
		);

	return * temp;
}
