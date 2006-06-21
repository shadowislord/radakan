#include "inventory.hpp"

using namespace std;

//  Constructor
Inventory::
	Inventory (string new_name):
	Object::
	Object (new_name)
{
	assert (Object::is_initialized ());
	
	assert (is_initialized ());
}

//  Destructor
Inventory::
	~Inventory ()
{
	assert (is_initialized ());
}

