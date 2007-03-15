#include "static_item.hpp"

using namespace std;
using namespace tsl;

//  constructor
Static_Item ::
	Static_Item
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_mass
	) :
	Object (new_name),
	Item
	(
		new_mesh_name,
		new_volume,
		new_mass,
		false,
		true,
		true
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
	
	OgreOde :: Geometry * geometry;

	if (find ("Ground") != string :: npos)
	{
		OgreOde :: EntityInformer * ei = new OgreOde :: EntityInformer (& entity, Ogre :: Matrix4 ());
		geometry = ei -> createStaticTriangleMesh (& Environment :: get (), Environment :: get () . getDefaultSpace ());
		log (debugging) << "A static triangle mesh was created for " << string :: data () << "." << endl;
	}
	else
	{
		geometry = new OgreOde :: SphereGeometry (Ogre :: Math :: RangeRandom (0.5, 1.5), & Environment :: get (), Environment :: get () . getDefaultSpace ());
		log (debugging) << "A default sphere mesh was created for " << string :: data () << "." << endl;
	}

	return * geometry;
}

//	static
Item & Static_Item ::
	create
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_mass
	)
{
	Item * temp =
		new Static_Item
		(
			new_name,
			new_mesh_name,
			new_volume,
			new_mass
		);

	return * temp;
}
