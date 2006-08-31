#include "container.hpp"

using namespace std;

//  Constructor
Container::
	Container (string new_name):
	Obstacle (new_name, Vector3 (0.3, 0.3, 0.3), true, 30, 3)
{
	assert (Object::is_initialized ());
	
	assert (is_initialized ());
}

//  Destructor
Container::
	~Container ()
{
	assert (is_initialized ());
}

bool Container::add (Obstacle * item)
{
	assert (is_initialized ());

	//	second means we're interested in if it worked or not.
	//	first would give a iterator to the item
	return items.insert (item).second;
}

bool Container::remove (Obstacle * item)
{
	assert (is_initialized ());

	return (0 < items.erase (item));
}

bool Container::contains (Obstacle * item)
{
	assert (is_initialized ());

	return (items.find (item) != items.end ());
}

//	virtual
float
	Container::
	get_total_weight ()
	const
{
	float total_weight = weight;

	for (set <Obstacle *> :: iterator i = items.begin (); i != items.end (); i ++)
	{
		total_weight += (* i)->get_total_weight ();
	}

	return total_weight;
}
