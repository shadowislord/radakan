#include "player.hpp"

using namespace std;
using namespace sl;

template <> Player * Ogre :: Singleton <Player> :: ms_Singleton = NULL;

//  constructor
Player ::
	Player (Ogre :: SceneNode * new_node) :
	Object (get_name (new_node)),
	Character (new_node)
{
	assert (Character :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
Player ::
	~Player ()
{
	assert (Object :: is_initialized ("~Player ()"));
}

//	virtual
bool Player ::
	is_initialized ()
	const
{
	return Character :: is_initialized ();
}
