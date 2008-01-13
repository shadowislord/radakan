#include "engines/log.hpp"
#include "items/static_item.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;

//	static
string Static_Item ::
	get_class_name ()
{
	return "Static_Item";
}

//	static
unsigned int Static_Item ::
	counter (0);

//  constructor
Static_Item ::
	Static_Item
	(
		float new_mass,
		Ogre :: Vector3 new_size,
		const Reference <Mesh_Data> new_mesh_data
	) :
	Object ("static item " + to_string (counter ++) + " (based on " + new_mesh_data . get_name () + ")"),
	Item
	(
		new_mass,
		new_size,
		new_mesh_data
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
