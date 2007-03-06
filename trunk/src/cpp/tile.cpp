#include "tile.hpp"

using namespace std;
using namespace tsl;

const int Tile :: side_length (20);

Tile ::
	Tile (OgreOde :: World & new_world, pair <int, int> new_coordinates, string tsl_path) :
	Object (tsl_path + "/data/tile/tile_" + to_string (new_coordinates . first) + "_" + to_string (new_coordinates . second) + ".xml"),
	coordinates (new_coordinates),
	position (side_length * Ogre :: Vector3	(coordinates . first, 0, coordinates . second)),
	world (new_world),
	doc (new TiXmlDocument (string :: c_str ()))
{

	log (TSL_DEBUG) << "Tile (~new_world~, (" << to_string (new_coordinates . first) << ", " << to_string (new_coordinates . second) << "), " << tsl_path << ")" << endl;
	
	//	CEGUI-0.4.1 is linked to TinyXml-2.3.3, so we have to use that version.
	assert (TIXML_MAJOR_VERSION == 2);
	assert (TIXML_MINOR_VERSION == 3);
	assert (TIXML_PATCH_VERSION == 3);

	assert (Object :: is_initialized ());

	TiXmlDocument & document = * doc;
	bool check = document . LoadFile ();
	assert (check);
	assert (! document . Error ());
	TiXmlElement * root = document . RootElement ();
	assert (root != NULL);
	for (TiXmlElement * body_xml = root -> FirstChildElement ("model");
				body_xml != NULL; body_xml = body_xml -> NextSiblingElement ("model"))
	{
		add_xml (* body_xml);
	}

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
					Body & pine_tree = represent
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

	assert (is_initialized ());

	log (TSL_DEBUG) << "Tile (~new_world~, (" << to_string (new_coordinates . first) << ", " << to_string (new_coordinates . second) << "), " << tsl_path << ") V" << endl;

}

Tile ::
	~Tile ()
{
	log (TSL_DEBUG) << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool Tile ::
	is_initialized ()
	const
{
	assert (warn <Tile> (Disjoint_Set <Item> :: is_initialized ()));

	return true;
}

//	static
string Tile ::
	get_class_name ()
{
	return "Tile";
}

//	virtual
bool Tile ::
	add (Item & item)
{
	assert (is_initialized ());
	assert (item . is_initialized ());
	assert (item . has_body ());
	assert (! contains (item));

	bool check = Disjoint_Set <Item> :: add (item);
	assert (check);

	if (item . is_type <NPC> ())
	{
		log (TSL_DEBUG) << item << " will be added to the list of NPCs..." << endl;
		bool check = npcs . insert (& item . to_type <NPC> ()) . second;
		assert (check);
		
		log (TSL_DEBUG) << item << " was added to the list of NPCs." << endl;
	}
	
	return true;
}

//	virtual
bool Tile ::
	move (Item & item, Disjoint_Set <Item> & destination)
{
	log (TSL_DEBUG) << "move (" << item << ", " << destination << ")" << endl;
	assert (is_initialized ());
	assert (item . is_initialized ());
	assert (item . has_body ());
	assert (contains (item));
	assert (destination . is_initialized ());

	if (item . is_type <NPC> ())
	{
		npcs . erase (& item . to_type <NPC> ());
	}

	bool check = Disjoint_Set <Item> :: move (item, destination);
	assert (check);

	return true;
}

void Tile ::
	add_xml (TiXmlElement & element)
{
	log (TSL_DEBUG) << "add_xml (~element~)" << endl;
	assert (is_initialized ());

	float x = to_float (element . Attribute ("x"));
	float y = to_float (element . Attribute ("y"));
	float z = to_float (element . Attribute ("z"));
	float scale = to_float (element . Attribute ("scale"));

	TiXmlElement * item_xml = element . FirstChildElement ();
	
	//	I'm not sure why 'string' is necessairy here. --Tinus
	assert ((item_xml -> Value () == string ("item")) || (item_xml -> Value () == string ("npc")));

	string name = item_xml -> Attribute ("name") + string (" (") + * this + ")";
	string mesh = item_xml -> Attribute ("mesh");
	float volume = to_float (item_xml -> Attribute ("volume"));
	float mass = to_float (item_xml -> Attribute ("mass"));
	bool mobile = item_xml -> Attribute ("mobile") == string ("true");
	//	bool solid = item_xml -> Attribute ("solid") == string ("true");
	//	bool visible = item_xml -> Attribute ("visible") == string ("true");

	Item * item = NULL;
	if (item_xml -> Value () == string ("item"))
	{
		assert (! mobile);
		item = & Static_Item :: create (name, mesh, volume, mass);
	}
	else if (item_xml -> Value () == string ("npc"))
	{
		assert (mobile);
		item = & NPC :: create (name, mesh, volume, mass);
	}
	else
	{
		error () << "didn't recognize xml tag name: " << item_xml -> Value () << endl;
		abort ();
	}

	item -> set_body (world);
	add (* item);

	assert (item -> has_body ());
	Body & body = item -> get_body ();
	
	body . setPosition (position + Ogre :: Vector3 (x, y, z));
	body . set_scale (scale);

	TiXmlElement * material = element . FirstChildElement ("material");
	if (material != NULL)
	{
		body . set_material (material -> Attribute ("name"));
	}
}
