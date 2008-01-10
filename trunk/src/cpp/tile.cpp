#include <tinyxml.h>

#include <Ogre.h>
#include <OgreStringConverter.h>
#include <OgreException.h>

#include <OgreOdeBody.h>
#include <OgreOdeGeometry.h>
#include <OgreOdeSpace.h>
#include <OgreOdeWorld.h>

#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "items/npc.hpp"
#include "items/player_character.hpp"
#include "items/static_item.hpp"
#include "movable_model.hpp"
#include "set.hpp"
#include "tile.hpp"
#include "world.hpp"

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
	side_length (20);

Tile ::
	Tile (pair <int, int> new_coordinates) :
	Object
	(
		"tile_" + to_string (new_coordinates . first) + "_" + to_string (new_coordinates . second)
	),
	coordinates (new_coordinates),
	position (side_length * Ogre :: Vector3	(coordinates . first, 0, coordinates . second)),
	npcs (new Set <Items :: NPC> (name + "'s NPCs")),
	space (new OgreOde :: SimpleSpace (World :: get () -> ogre_ode_world . get (), World :: get () -> ogre_ode_world -> getDefaultSpace ())),
	doc (new TiXmlDocument (Engines :: Settings :: get () -> radakan_path + "/data/world/tile/" + name + ".xml"))
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "", "(" + to_string (new_coordinates . first) + ", " + to_string (new_coordinates . second) + ")");

	load_tile_file (* doc);

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

	//	set a floor
	new OgreOde :: InfinitePlaneGeometry (Ogre :: Plane (y_axis, 0), World :: get () -> ogre_ode_world . get (), space . get ());

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
	assert (Location <Model> :: is_initialized ());

	return true;
}

//	virtual
bool Tile ::
	add (Reference <Model> model)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "add", model ->name);
	assert (is_initialized ());
	assert (model . points_to_object ());
	assert (model -> is_initialized ());
	assert (! contains (model));

	bool check = Location <Model> :: add (model);
	assert (check);

	if (model -> item . is_castable <Items :: NPC> ())
	{
		Engines :: Log :: log (me) << model -> item << " will be added to the list of NPCs..." << endl;
		assert (model -> item . points_to_object ());
		assert (model -> item -> is_initialized ());
		Reference <Items :: NPC> temp (model -> item . cast <Items :: NPC> ());
		assert (temp . points_to_object ());
		assert (temp -> is_initialized ());
		
		bool check = npcs -> add (temp);
		assert (check);
		
		Engines :: Log :: log (me) << model -> item << " was added to the list of NPCs." << endl;
	}

	model -> set_space (space);
	
	return true;
}

//	virtual
bool Tile ::
	move (Reference <Model> model, Reference <Container <Model> > destination)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "move", model . get_name (), destination . get_name ());
	assert (is_initialized ());
	assert (model -> is_initialized ());
	assert (contains (model));
	assert (destination -> is_initialized ());

	if (model -> item . is_castable <Items :: NPC> ())
	{
		npcs -> drop (model -> item . cast <Items :: NPC> ());
	}

	bool check = Location <Model> :: move (model, destination);
	assert (check);

	return true;
}

void Tile ::
	load_model (TiXmlElement & element)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "load_model", element . Value ());
	assert (is_initialized ());

	assert (element . Attribute ("item") != NULL);
	TiXmlDocument item_prototype
	(
		Engines :: Settings :: get () -> radakan_path + "/data/world/prototype/" + element . Attribute ("item") + ".xml"
	);
	Reference <Items :: Item> item (load_item_prototype_file (item_prototype));

	//	'position' is the position of this tile.
	Ogre :: Vector3 model_position = position + Ogre :: Vector3
	(
		to_float (element . Attribute ("x")),
		to_float (element . Attribute ("y")),
		to_float (element . Attribute ("z"))
	);

	Pointer <Model> model;
	if (item . is_castable <Items :: Static_Item> ())
	{
		model . reset_pointee (new Model (item, model_position));
	}
	else
	{
		model . reset_pointee (new Movable_Model (item, model_position));
	}
	
	bool check = add (model);
	assert (check);

	//	TODO re-enable assert ((position + Ogre :: Vector3 (x, y, z) - model . getPosition ()) . length () < 0.01);
}

Reference <Items :: Item> Tile ::
	load_item_prototype_file (TiXmlDocument & document)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "load_item_prototype_file", document . Value ());
	assert (is_initialized ());
	
	bool check = document . LoadFile ();
	assert (check);
	assert (! document . Error ());
	TiXmlElement * item_element = document . RootElement ();
	
	Engines :: Log :: log (me) << "item_element: " << item_element -> Value () << endl;

	string type = item_element -> Attribute ("type");
	float mass = to_float (item_element -> Attribute ("mass"));

	TiXmlElement * size_element = item_element -> FirstChildElement ("size");
	assert (size_element != NULL);
	Ogre :: Vector3 size = Ogre :: Vector3
	(
		to_float (size_element -> Attribute ("x")),
		to_float (size_element -> Attribute ("y")),
		to_float (size_element -> Attribute ("z"))
	);

	//	TODO Do this smarter so we don't have to re-run this every time.
	TiXmlElement * mesh_element = item_element -> FirstChildElement ("mesh");

	string mesh_file_name = mesh_element -> Attribute ("file_name");

	Reference <Mesh_Data> mesh_data (new Mesh_Data (mesh_file_name));

	if (mesh_element -> Attribute ("material") != NULL)
	{
		mesh_data -> material_file_name = mesh_element -> Attribute ("material");
	}
	if (mesh_element -> Attribute ("scale") != NULL)
	{
		mesh_data -> scale = to_float (mesh_element -> Attribute ("scale"));
	}
	if (mesh_element -> Attribute ("solid") != NULL)
	{
		mesh_data -> solid = mesh_element -> Attribute ("solid") == string ("true");
	}
	if (mesh_element -> Attribute ("visible") != NULL)
	{
		mesh_data -> visible = mesh_element -> Attribute ("visible") == string ("true");
	}

	if (mesh_element -> Attribute ("rotation") != NULL)
	{
		if (mesh_element -> Attribute ("rotation") == string ("true"))
		{
			mesh_data -> default_orientation = make_quaternion (- Ogre :: Math :: HALF_PI, x_axis);
		}
	}

	Pointer <Items :: Item> item;
	if (type == string ("static_item") || type == string ("plane"))
	{
		item . reset_pointee (new Items :: Static_Item (mass, size, mesh_data));
	}
	else
	{
		if (type == string ("npc"))
		{
			item . reset_pointee (new Items :: NPC (name, mass, size, mesh_data));
		}
		else if (type == string ("player"))
		{
			item . reset_pointee (new Items :: Player_Character (name, mass, size, mesh_data));
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

void Tile ::
	load_tile_file (TiXmlDocument & document)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "load_xml_file", document . Value ());
	assert (is_initialized ());

	bool check = document . LoadFile ();
	assert (check);
	assert (! document . Error ());
	TiXmlElement * root = document . RootElement ();

	for (TiXmlElement * xml_element = root -> FirstChildElement ();
		xml_element != NULL; xml_element = xml_element -> NextSiblingElement ())
	{
		load_model (* xml_element);
	}
}
