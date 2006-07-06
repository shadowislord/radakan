#include "inventory.hpp"

using namespace std;

//  Constructor
Inventory::
	Inventory (string new_name):
	//Object::
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

bool Inventory::add (Obstacle * item)
{
	assert (is_initialized ());

	int index = find (item);
	
	if (0 <= index)
	{
		return false;
	}
	
	items.push_back (item);
	return true;
}

bool Inventory::remove (Obstacle * item)
{
	assert (is_initialized ());

	int index = find (item);
	
	if (index < 0)
	{
		return false;
	}
	
	items.at (index) = NULL;
	return true;
}

bool Inventory::contains (Obstacle * item)
{
	assert (is_initialized ());

	if (find (item) < 0)
	{
		return false;
	}
	
	return true;
}

int Inventory::find (Obstacle * item)
{
	assert (is_initialized ());

	for (unsigned int i = 0; i < items.size (); i++)
	{
		if (items.at (i) == item)
		{
			return i;
		}
	}

	return - 1;
}
