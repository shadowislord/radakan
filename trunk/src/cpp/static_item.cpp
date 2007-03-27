#include "static_item.hpp"

using namespace std;
using namespace tsl;

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
	assert (Item :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
Static_Item ::
	~Static_Item ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool Static_Item ::
	is_initialized ()
	const
{
	return warn <Static_Item> (Item :: is_initialized ());
}

//	static
string Static_Item ::
	get_class_name ()
{
	return "Static_Item";
}

//	virtual
OgreOde :: Geometry & Static_Item ::
	create_geometry ()
{
	assert (is_initialized ());
	assert (! has_body ());
	
	if (find ("Ground") != string :: npos)
	{
		OgreOde :: EntityInformer ei (& entity);
		return * ei . createStaticTriangleMesh (& Environment :: get (), Environment :: get () . getDefaultSpace ());
	}
	
	return Item :: create_geometry ();
}

//	static
Item & Static_Item ::
	create
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass,
		bool new_solid,
		bool new_visible
	)
{
	Item * temp =
		new Static_Item
		(
			new_name,
			new_mesh_name,
			new_size,
			new_mass,
			new_solid,
			new_visible
		);

	return * temp;
}
