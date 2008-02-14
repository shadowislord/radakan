#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "items/character.hpp"
#include "items/static_item.hpp"
#include "set.hpp"
#include "tile.hpp"
#include "world.hpp"

#include <tinyxml.h>

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	#include <OgreOdeBody.h>
	#include <OgreOdeGeometry.h>
	#include <OgreOdeSpace.h>
#endif

using namespace std;
using namespace Radakan;

//	static
string Tile ::
	get_class_name ()
{
	return "Tile";
}

//	static
const int Tile ::
	side_length (64);

Tile ::
	Tile
	(
		Mathematics :: Vector_3D new_coordinates
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
		,
		boost :: shared_ptr <OgreOde :: SimpleSpace> new_space
#endif
	) :
	Object ("tile at " + new_coordinates . to_string ()),
	coordinates (new_coordinates),
	position (side_length * coordinates),
	characters
	(
		new Set <Items :: Character>
		(
			me . get_name () + "'s characters",
			Container <Items :: Character> :: unlimited (),
			true
		)
	),
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	space (new_space),
#endif
	doc (new TiXmlDocument
		(Engines :: Settings :: get () -> radakan_path + "/data/tile/tile_"
			+ to_string (coordinates . x) + "_" + to_string (coordinates . z) + ".xml"))
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "", coordinates . to_string ()
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	, "@new_space@"
#endif
	);

	bool check = doc -> LoadFile ();
	assert (check);
	assert (! doc -> Error ());
	TiXmlElement * root = doc -> RootElement ();

	add_automatic_destruction_prevention ("construction of " + get_class_name ());
	for (TiXmlElement * xml_element = root -> FirstChildElement ();
		xml_element != NULL; xml_element = xml_element -> NextSiblingElement ())
	{
		load_model (* xml_element);
	}
	remove_automatic_destruction_prevention ("construction of " + get_class_name ());


/*	if (root -> FirstChildElement ("forest") != NULL)
	{
		//	forest of 1961 trees
		//	(30000 trees takes to long to load)
		const int trees_one_a_line = 11;

		float x;
		const float y = 0;
		float z;
		const float scale = 0.06;
		string tree_name;

		for (int i = 0; i < trees_one_a_line; i ++)
		{
			for (int j = 0; j < trees_one_a_line; j ++)
			{
				x = side_length * i / trees_one_a_line + Ogre :: Math :: RangeRandom (0.5, 0.5);
				z = side_length * j / trees_one_a_line + Ogre :: Math :: RangeRandom (0.5, 0.5);
				tree_name = "Tree no. " + to_string (trees_one_a_line * i + j);

				if (0.4 < Ogre :: Math :: RangeRandom (0, 1))
				{
					represent
					(
						Item :: create (tree_name, "tree.mesh", 0, 0, false),
						x,
						y,
						z,
						scale
					) . turn (- Ogre :: Math :: HALF_PI, Ogre :: Vector3 (1, 0, 0));
				}
				else
				{
					Model & pine_tree = represent
					(
						Item :: create (tree_name, "pine_tree.mesh", 0, 0, false),
						x,
						y,
						z,
						scale
					);
					pine_tree . move (4, pine_tree . get_top_direction ());
				}
			}
		}
	}*/

	/*
	//	not textured
	represent (Item :: create ("House", "house.mesh", 0, 0, false), 42, 0, 18, 0.016);
	represent (Item :: create ("Wagon", "wagon.mesh", 0, 0), - 10, 0, - 10, 0.0008);
	represent (Item :: create ("Pot", "pot.mesh", 0, 0), - 12, 0, - 10, 0.0008);
	represent (Item :: create ("Pot 2", "pot_2.mesh", 0, 0), - 14, 0, - 10, 0.0008);
	represent (Item :: create ("Pine tree", "pine_tree_2.mesh", 0, 0, false), - 18, 1.4, - 18, 0.06);
	*/

	Engines :: Log :: log (me) << "Tile loaded" << endl;

	assert (is_initialized ());
}

Tile ::
	~Tile ()
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Tile ::
	is_initialized ()
	const
{
	assert (Location <Items :: Item> :: is_initialized ());

	return true;
}

//	virtual
bool Tile ::
	add (Reference <Items :: Item> item)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "add", item . get_name ());
	assert (is_initialized ());
	assert (item . points_to_object ());
	assert (item -> is_initialized ());
	assert (! contains (item));

	bool check = Location <Items :: Item> :: add (item);
	assert (check);

	if (item . is_castable <Items :: Character> ())
	{
		bool check = characters -> add (item . cast <Items :: Character> ());
		assert (check);
	}

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	item -> set_space (space);
#endif

	return true;
}

//	virtual
bool Tile ::
	move (Reference <Items :: Item> item, Reference <Container <Items :: Item> > destination)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "move", item . get_name (), destination . get_name ());
	assert (is_initialized ());
	assert (item -> is_initialized ());
	assert (contains (item));
	assert (destination -> is_initialized ());

	if (item . is_castable <Items :: Character> ())
	{
		characters -> drop (item . cast <Items :: Character> ());
	}

	bool check = Location <Items :: Item> :: move (item, destination);
	assert (check);

	return true;
}

void Tile ::
	load_model (TiXmlElement & element)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "load_model", element . Value ());
	assert (is_initialized ());

	string item_name;
	if (element . Attribute ("name") != NULL)
	{
	    item_name = element . Attribute ("name");
	}

	assert (element . Attribute ("item") != NULL);
	TiXmlDocument item_prototype
	(
		Engines :: Settings :: get () -> radakan_path + "/data/item_prototype/" + element . Attribute ("item") + ".xml"
	);
	Reference <Items :: Item> item (load_item (item_prototype, item_name));

	//	'position' is the position of this tile.
	Mathematics :: Vector_3D model_position = position + Mathematics :: Vector_3D
		(element . Attribute ("position"));

	item -> appear (model_position, Mathematics :: Quaternion :: identity);

	bool check = add (item);
	assert (check);
}

Reference <Items :: Item> Tile ::
	load_item (TiXmlDocument & document, string item_name)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "load_item", document . Value ());
	assert (is_initialized ());

	bool check = document . LoadFile ();
	assert (check);
	assert (! document . Error ());
	TiXmlElement * item_element = document . RootElement ();

	Engines :: Log :: log (me) << "item_element: " << item_element -> Value () << endl;

	string type = item_element -> Attribute ("type");
	float mass = 0;
	if (item_element -> Attribute ("mass") != NULL)
	{
	    mass = to_float (item_element -> Attribute ("mass"));
	}
	Mathematics :: Vector_3D size (item_element -> Attribute ("size"));

	//	TODO Do this smarter so we don't have to re-run this every time.
	TiXmlElement * mesh_element = item_element -> FirstChildElement ("mesh");

	string mesh_file_name = mesh_element -> Attribute ("file_name");

	Reference <Mesh_Data> mesh_data (new Mesh_Data (mesh_file_name));

	mesh_element -> QueryFloatAttribute ("scale", & mesh_data -> scale);
	if (mesh_element -> Attribute ("material") != NULL)
	{
	    mesh_data -> material_file_name = mesh_element -> Attribute ("material");
	}

	bool solid = true;
	if (mesh_element -> Attribute ("solid") != NULL)
	{
		solid = mesh_element -> Attribute ("solid") == string ("true");
	}

	bool visible = true;
	if (mesh_element -> Attribute ("visible") != NULL)
	{
		visible = mesh_element -> Attribute ("visible") == string ("true");
	}

	if (mesh_element -> Attribute ("orientation") != NULL)
	{
		mesh_data -> default_orientation
			= Mathematics :: Quaternion (mesh_element -> Attribute ("orientation"));
	}

	Pointer <Items :: Item> item;
	if (type == string ("static_item") || type == string ("plane"))
	{
		assert (item_name . empty ());

		item . reset_pointee (new Items :: Static_Item (mass, size, mesh_data));
	}
	else
	{
		if ((type == string ("npc")) || (type == string ("player character")))
		{
			assert (! item_name . empty ());

			item = Reference <Items :: Item>
				(new Items :: Character (item_name, mass, size, mesh_data, type));
		}
		else
		{
			Engines :: Log :: error (me) << "Unrecognized item type: " << type << endl;
			abort ();
		}
	}
	assert (item . points_to_object ());
	assert (item -> is_initialized ());

	return item;
}
