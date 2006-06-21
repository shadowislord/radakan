#include "character.hpp"

using namespace std;

//  Constructor
Character::
	Character (string new_name, Vector3 new_position/*, D3 new_size*/):
	Obstacle::
	Obstacle (new_name, new_position/*, new_size*/)
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
}
