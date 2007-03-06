#include "player.hpp"

using namespace std;
using namespace tsl;

//  constructor
Player ::
	Player
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_mass
	) :
	Object (new_name),
	Character
	(
		new_mesh_name,
		new_volume,
		new_mass
	),
	camera_distance (0.72),
	dead (false)
{
	log (TSL_DEBUG) << "Player (" << new_name << ", ...)" << endl;
	assert (Character :: is_initialized ());

/*	bool check = back . add (Container :: create ("Backbpack", "bar.mesh", 30, 3));
	assert (check);

	check = hands . add (Weapon :: create ("Sword", "bar.mesh", 1, 2, 3, 4, 5, 6, 7, 8));
	assert (check);*/

	assert (is_initialized ());
}

//  destructor
Player ::
	~Player ()
{
	log (TSL_DEBUG) << "~" << get_class_name () << " ()" << endl;
	assert (Object :: is_initialized ());
}

//	virtual
bool Player ::
	is_initialized ()
	const
{
	return Character :: is_initialized ();
}

//	static
string Player ::
	get_class_name ()
{
	return "Player";
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
	dead = true;

	log () << string :: data () << " died!";
}

//	static
Item & Player ::
	create
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_mass
	)
{
	Item * temp =
		new Player
		(
			new_name,
			new_mesh_name,
			new_volume,
			new_mass
		);

	return * temp;
}
