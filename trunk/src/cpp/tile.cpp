#include "tile.hpp"

using namespace std;
using namespace tsl;

const int Tile :: side_length (20);

Tile ::
	Tile (pair <int, int> new_coordinates, string new_tsl_path) :
	Object
	(
		"tile_" + to_string (new_coordinates . first) + "_" + to_string (new_coordinates . second)
	),
	OgreOde :: SimpleSpace (& Environment :: get (), Environment :: get () . getDefaultSpace ()),
	coordinates (new_coordinates),
	position (side_length * Ogre :: Vector3	(coordinates . first, 0, coordinates . second)),
	tsl_path (new_tsl_path),
	doc ((tsl_path + "/data/tile/" + string :: c_str () + ".xml") . c_str ())
{
	log (debugging) << get_class_name () << " :: " << get_class_name () << " ((" << to_string (new_coordinates . first) << ", " << to_string (new_coordinates . second) << "), " << tsl_path << ")" << endl;
	
	//	CEGUI-0.4.1 is linked to TinyXml-2.3.3, so we have to use that version.
	assert (TIXML_MAJOR_VERSION == 2);
	assert (TIXML_MINOR_VERSION == 3);
	assert (TIXML_PATCH_VERSION == 3);

	assert (Object :: is_initialized ());

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

	//	set a floor
	new OgreOde :: InfinitePlaneGeometry (Ogre :: Plane (y_axis, 0), & Environment :: get (), this);

	assert (is_initialized ());
}

Tile ::
	~Tile ()
{
	log (debugging) << get_class_name () << " :: ~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool Tile ::
	is_initialized ()
	const
{
	assert (Disjoint_Set <Body> :: is_initialized ());

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
	add (Body & body)
{
	log (debugging) << get_class_name () << " :: add (" << body << ")" << endl;
	assert (is_initialized ());
	assert (body . is_initialized ());
	assert (! contains (body));

	bool check = Disjoint_Set <Body> :: add (body);
	assert (check);

	if (body . item . is_type <NPC> ())
	{
		log (debugging) << body . item << " will be added to the list of NPCs..." << endl;
		bool check = npcs . insert (& body . item . to_type <NPC> ()) . second;
		assert (check);
		
		log (debugging) << body . item << " was added to the list of NPCs." << endl;
	}

	if (body . geometry . getSpace () != NULL)
	{
		body . geometry . getSpace () -> removeGeometry (body . geometry);
	}
	OgreOde :: Space :: addGeometry (body . geometry);
	
	return true;
}

//	virtual
bool Tile ::
	move (Body & body, Disjoint_Set <Body> & destination)
{
	log (debugging) << get_class_name () << " :: move (" << body << ", " << destination << ")" << endl;
	assert (is_initialized ());
	assert (body . is_initialized ());
	assert (contains (body));
	assert (destination . is_initialized ());

	if (body . item . is_type <NPC> ())
	{
		npcs . erase (& body . item . to_type <NPC> ());
	}

	bool check = Disjoint_Set <Body> :: move (body, destination);
	assert (check);

	return true;
}

void Tile ::
	load_xml (TiXmlElement & element)
{
	log (debugging) << get_class_name () << " :: load_xml (~element~)" << endl;
	assert (is_initialized ());

	if (element . Value () == string ("include"))
	{
		TiXmlDocument document ((tsl_path + "/data/tile/" + element . Attribute ("name") + ".xml") . c_str ());
		load_xml_file (document);
		return;
	}

	float x = to_float (element . Attribute ("x"));
	float y = to_float (element . Attribute ("y"));
	float z = to_float (element . Attribute ("z"));
	float scale = to_float (element . Attribute ("scale"));

	TiXmlElement * item_xml = element . FirstChildElement ();
	
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

	Item * item = NULL;
	if (item_xml -> Value () == string ("static_item"))
	{
		item = & Static_Item :: create (name, mesh, volume, mass, solid);
	}
	else if (item_xml -> Value () == string ("npc"))
	{
		item = & NPC :: create (name, mesh, volume, mass);
	}
	else if (item_xml -> Value () == string ("player"))
	{
		item = & Player :: create (name, mesh, volume, mass);
	}
	else
	{
		error () << "didn't recognize xml tag name: " << item_xml -> Value () << endl;
		abort ();
	}

	Body & body = * new Body (* item, position + Ogre :: Vector3 (x, y, z), scale);
	add (body);

	//	TODO re-enable assert ((position + Ogre :: Vector3 (x, y, z) - body . getPosition ()) . length () < 0.01);

	TiXmlElement * material = element . FirstChildElement ("material");
	if (material != NULL)
	{
		body . set_material (material -> Attribute ("name"));
	}
}

void Tile ::
	load_xml_file (TiXmlDocument & document)
{
	log (debugging) << get_class_name () << " :: load_xml_file (~document~)" << endl;
	assert (is_initialized ());

	bool check = document . LoadFile ();
	assert (check);
	assert (! document . Error ());
	TiXmlElement * root = document . RootElement ();

	for (TiXmlElement * body_xml = root -> FirstChildElement ();
				body_xml != NULL; body_xml = body_xml -> NextSiblingElement ())
	{
		load_xml (* body_xml);
	}
}
