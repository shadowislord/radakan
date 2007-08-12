#include "log.hpp"
#include "static_item.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Items;

//	static
const string Static_Item ::
	get_class_name ()
{
	return "Static_Item";
}

//  constructor
Static_Item ::
	Static_Item
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass,
		bool new_solid,
		bool new_visible
	) :
	Object (new_name),
	Item
	(
		new_mesh_name,
		new_size,
		new_mass,
		false,
		new_solid,
		new_visible
	)
{
	//	Do nothing.

	assert (is_initialized ());
}

//  destructor
Static_Item ::
	~Static_Item ()
{
	Engines :: Log :: trace (me, Static_Item :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Static_Item ::
	is_initialized ()
	const
{
	return Item :: is_initialized ();
}
