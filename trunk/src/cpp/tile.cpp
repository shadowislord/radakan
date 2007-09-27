#include <tinyxml.h>

#include <Ogre.h>
#include <OgreStringConverter.h>
#include <OgreException.h>

#include <OgreOdeBody.h>
#include <OgreOdeGeometry.h>
#include <OgreOdeSpace.h>
#include <OgreOdeWorld.h>

#include "log.hpp"
#include "movable_model.hpp"
#include "npc.hpp"
#include "player_character.hpp"
#include "static_item.hpp"
#include "tile.hpp"
#include "settings.hpp"
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
	doc (new TiXmlDocument (Engines :: Settings :: get () -> radakan_path + "/data/tile/" + name + ".xml"))
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "", "(" + to_string (new_coordinates . first) + ", " + to_string (new_coordinates . second) + ")");
	
	load_xml_file (doc);

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
	assert (Set <Model> :: is_initialized ());

	return true;
}

//	virtual
bool Tile ::
	add (Reference <Model> model)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "add", model ->name);
	assert (is_initialized ());
	assert (model -> is_initialized ());
	assert (! contains (model));

	bool check = Location <Model> :: add (model);
	assert (check);

	if (model -> item -> is_type <Items :: NPC> ())
	{
		Engines :: Log :: log (me) << model -> item -> name << " will be added to the list of NPCs..." << endl;
		bool check = npcs -> add (model -> item -> to_type <Items :: NPC> ());
		assert (check);
		
		Engines :: Log :: log (me) << model -> item -> name << " was added to the list of NPCs." << endl;
	}

	model -> set_space (space);
	
	return true;
}

//	virtual
bool Tile ::
	move (Reference <Model> model, Reference <Set <Model> > destination)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "move", model -> name, destination -> name);
	assert (is_initialized ());
	assert (model -> is_initialized ());
	assert (contains (model));
	assert (destination -> is_initialized ());

	if (model -> item -> is_type <Items :: NPC> ())
	{
		npcs -> drop (model -> item -> to_type <Items :: NPC> ());
	}

	bool check = Set <Model> :: move (model, destination);
	assert (check);

	return true;
}

void Tile ::
	load_xml (boost :: shared_ptr <TiXmlElement> element)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "load_xml", "~element~");
	assert (is_initialized ());

	Engines :: Log :: log (me) << "element value: " << element -> ValueStr () << endl;
	if (element -> ValueStr () == string ("include"))
	{
		boost :: shared_ptr <TiXmlDocument> document
		(
			new TiXmlDocument
			(
				Engines :: Settings :: get () -> radakan_path + "/data/tile/" + element -> Attribute ("name") + ".xml"
			)
		);
		load_xml_file (document);
		return;
	}

//	float x = to_float (element . Attribute ("x"));
/*	double x;
	element . Attribute ("x", & x);
	Engines :: Log :: log (me) << "x: " << x << endl;*/
	float x = to_float (element -> Attribute ("x"));
	float y = to_float (element -> Attribute ("y"));
	float z = to_float (element -> Attribute ("z"));
	float scale = to_float (element -> Attribute ("scale"));

	TiXmlElement * item_xml = element -> FirstChildElement ();
	
	string name = item_xml -> Attribute ("name") + string (" ") + to_string (position);
	string mesh = item_xml -> Attribute ("mesh");
	float mass = to_float (item_xml -> Attribute ("mass"));
	bool solid = item_xml -> Attribute ("solid") == string ("true");
	//	bool visible = item_xml -> Attribute ("visible") == string ("true");

	Ogre :: Vector3 volume;
	TiXmlElement * volume_element = item_xml -> FirstChildElement ("volume");
	assert (volume_element != NULL);
	volume = Ogre :: Vector3
	(
		to_float (volume_element -> Attribute ("x")),
		to_float (volume_element -> Attribute ("y")),
		to_float (volume_element -> Attribute ("z"))
	);

	Reference <Items :: Item> item;
	if (item_xml -> ValueStr () == string ("static_item"))
	{
		item . reset_pointee (new Items :: Static_Item (name, mesh, volume, mass, solid));
	}
	else if (item_xml -> ValueStr () == string ("plane"))
	{
		item . reset_pointee (new Items :: Static_Item (name, mesh, volume, mass, solid));
	}
	else if (item_xml -> ValueStr () == string ("npc"))
	{
		item . reset_pointee (new Items :: NPC (name, mesh, volume, mass));
	}
	else if (item_xml -> ValueStr () == string ("player"))
	{
		item . reset_pointee (new Items :: Player_Character (name, mesh, volume, mass));
	}
	else
	{
		Engines :: Log :: error (me) << "Unrecognizable xml tag name: " << item_xml -> ValueStr () << endl;
		abort ();
	}

	Reference <Model> model = create_model (item, position + Ogre :: Vector3 (x, y, z), scale);
	bool check = add (model);
	assert (check);

	//	TODO re-enable assert ((position + Ogre :: Vector3 (x, y, z) - model . getPosition ()) . length () < 0.01);

	TiXmlElement * material = element -> FirstChildElement ("material");
	if (material != NULL)
	{
		model -> set_material (material -> Attribute ("name"));
	}
}

void Tile ::
	load_xml_file (boost :: shared_ptr <TiXmlDocument> document)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "load_xml_file", "~document~");
	assert (is_initialized ());

	bool check = document -> LoadFile ();
	assert (check);
	assert (! document -> Error ());
	boost :: shared_ptr <TiXmlElement> root (document -> RootElement ());
	assert (root != NULL);
	
	for (boost :: shared_ptr <TiXmlElement> xml_element (root -> FirstChildElement ());
		xml_element; xml_element . reset (xml_element -> NextSiblingElement ()))
	{
		load_xml (xml_element);
	}
}

Reference <Model> Tile ::
	create_model (Reference <Items :: Item> item, Ogre :: Vector3 position, float scale)
{
	Engines :: Log :: trace (me, Tile :: get_class_name (), "create_model", item -> name, to_string (position), to_string (scale));
	boost :: shared_ptr <OgreOde :: Geometry> geometry = item -> create_geometry ();
	boost :: shared_ptr <OgreOde :: Body> body (geometry -> getBody ());
	if (body)
	{
		return Reference <Model> (new Movable_Model (item, position, scale, geometry, body));
	}
	else
	{
		return Reference <Model> (new Model (item, position, scale, geometry));
	}
}
