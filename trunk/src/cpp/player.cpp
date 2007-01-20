#include "player.hpp"

using namespace std;
using namespace tsl;

//  constructor
Player ::
	Player
	(
		float new_volume,
		float new_weight,
		btVector3 new_position,
		Ogre :: SceneNode & new_node
	) :
	Object (get_name (new_node)),
	Singleton <Player> (get_name (new_node)),
	Character
	(
		new_volume,
		new_weight,
		new_position,
		new_node
	)
{
	trace () << "Player (" << node << ")" << endl;
	assert (Character :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
Player ::
	~Player ()
{
	trace () << "~Player ()" << endl;
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
