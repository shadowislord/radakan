#include "player.hpp"

using namespace std;
using namespace tsl;

//  constructor
Player ::
	Player
	(
		string new_name,
		string mesh_name,
		float new_volume,
		float new_weight
	) :
	Object (new_name),
	Character
	(
		mesh_name,
		new_volume,
		new_weight
	),
	dead (false)
{
	trace () << "Player (" << new_name << ")" << endl;
	assert (Character :: is_initialized ());

	bool check = back . add (Container :: create ("Backbpack", "bar.mesh", 30, 3));
	assert (check);

	check = hands . add (Weapon :: create ("Sword", "bar.mesh", 1, 2, 3, 4, 5, 6, 7, 8));
	assert (check);

	assert (is_initialized ());
}

//  destructor
Player ::
	~Player ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
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
string Player ::
	die ()
{
	dead = true;

	return * this + "died!";
}

//	static
Item & Player ::
	create
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_weight
	)
{
	Item * temp =
		new Player
		(
			new_name,
			new_mesh_name,
			new_volume,
			new_weight
		);

	return * temp;
}
