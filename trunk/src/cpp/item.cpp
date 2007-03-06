#include "item.hpp"

using namespace std;
using namespace tsl;

Ogre :: SceneManager * Item :: scene_manager = NULL;

//  constructor
Item ::
	Item
	(
		string new_mesh_name,
		float new_volume,
		float new_mass,
		bool new_mobile,
		bool new_solid,
		bool new_visible
	) :
	Object ("The name doesn't matter is this is an abstact base class."),
	volume (new_volume),
	mass (new_mass),
	mobile (new_mobile),
	solid (new_solid),
	visible (new_visible),
	entity (* scene_manager -> createEntity
										(* this + "'s entity", new_mesh_name)),
	body (NULL)
{
	log (TSL_DEBUG) << get_class_name () << " (" << string :: data () << ", "
		<< volume << ", " << mass << ", " << bool_to_string (mobile) << ", "
		<< bool_to_string (solid) << ", " << bool_to_string (visible) << ")"
		<< endl;
	assert (Object :: is_initialized ());

	assert (Item :: is_initialized ());
}

//  destructor
Item ::
	~Item ()
{
	log (TSL_DEBUG) << "~" << get_class_name () << " ()" << endl;
	assert (Item :: is_initialized ());

	if (has_body ())
	{
		remove_body ();
	}
	
	assert (Object :: is_initialized ());
}

//	virtual
bool Item ::
	is_initialized ()
	const
{
	log (TSL_DEBUG) << get_class_name () << " :: is_initialized ()" << endl;
	assert (warn <Item> (Object :: is_initialized ()));
	log (TSL_DEBUG) << get_class_name () << " :: is_initialized () A" << endl;
	assert (warn <Item> (scene_manager != NULL));
	log (TSL_DEBUG) << get_class_name () << " :: is_initialized () B" << endl;
	assert (warn <Item> (0 <= volume));
	log (TSL_DEBUG) << get_class_name () << " :: is_initialized () C" << endl;
	assert (warn <Item> (0 <= mass));
	log (TSL_DEBUG) << get_class_name () << " :: is_initialized () D" << endl;
	
	return true;
}

//	static
string Item ::
	get_class_name ()
{
	return "Item";
}

//	virtual
float Item ::
	get_total_mass ()
	const
{
	assert (Item :: is_initialized ());
	
	return mass;
}

void Item ::
	set_body (OgreOde :: World & world)
{
	log (TSL_DEBUG) << "add_body (~world~)" << endl;
	assert (Item :: is_initialized ());
	assert (visible);
	assert (! has_body ());

	OgreOde :: Geometry * geometry;

	if (find ("Ground") != string :: npos)
	{
		OgreOde :: EntityInformer * ei = new OgreOde :: EntityInformer (& entity, Ogre :: Matrix4 ());
		geometry = ei -> createStaticTriangleMesh (& world, world . getDefaultSpace ());
		log (TSL_DEBUG) << "A static triangle mesh was created for " << string :: data () << "." << endl;
	}
	else
	{
		OgreOde :: Body * body = new OgreOde :: Body (& world, string :: data ());

		geometry = new OgreOde :: SphereGeometry (Ogre :: Math :: RangeRandom (0.5, 1.5), & world, world . getDefaultSpace ());
		log (TSL_DEBUG) << "A default sphere mesh was created for " << string :: data () << "." << endl;

		geometry -> setBody (body);

		body -> setMass (OgreOde :: SphereMass (mass, 1 /*TODO set the right radius*/));
	}

	body = new Body (* scene_manager, Object :: to_type <Observable <Body> > (), entity, * geometry);
}

bool Item ::
	has_body () const
{
	assert (Item :: is_initialized ());
	
	return (body != NULL);
}

void Item ::
	remove_body ()
{
	assert (Item :: is_initialized ());
	assert (has_body ());
	assert (body -> getCreator () != NULL);

	delete body;
//	body -> getCreator () -> destroySceneNode (body -> Ogre :: SceneNode :: getName ());
	body = NULL;
}

Body & Item ::
	get_body () const
{
	assert (Item :: is_initialized ());
	assert (has_body ());
	
	return * body;
}
