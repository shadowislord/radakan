#include "character.hpp"

using namespace std;
using namespace Ogre;

//  Constructor
Character::
	Character (string new_name):
	Obstacle (new_name, Vector3 (2, 1, 1), true, 80, 65)
{
	assert (Obstacle::is_initialized ());

	inventory = new Inventory (* this + "'s inventory");
	
	assert (is_initialized ());
}

//  Destructor
Character::
	~Character ()
{
	assert (is_initialized ());

	delete inventory;
}

//	virtual
bool
	Character::
	is_initialized
	()
	const
{
	return Obstacle::is_initialized ();
}
