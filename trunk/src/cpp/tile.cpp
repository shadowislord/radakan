#include "log.hpp"
#include "tile.hpp"

using namespace std;
using namespace TSL;

//	static
const string Tile ::
	get_class_name ()
{
	return "Tile";
}

//	static
const int Tile ::
	side_length (20);

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
	npcs (my + "NPCs"),
	doc (tsl_path + "/data/tile/" + me + ".xml")
{
	Log :: trace <Tile> (me, "", " (" + to_string (new_coordinates . first) + ", " + to_string (new_coordinates . second) + ")", tsl_path);
	
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
	new OgreOde :: InfinitePlaneGeometry (Ogre :: Plane (y_axis, 0), & Environment :: get (), this);

	assert (is_initialized ());
}

Tile ::
	~Tile ()
{
	Log :: trace <Tile> (me, "~");
	assert (is_initialized ());
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
	add (Model & model)
{
	Log :: trace <Tile> (me, "add", model);
	assert (is_initialized ());
	assert (model . is_initialized ());
	assert (! contains (model));

	bool check = Set <Model> :: add (model);
	assert (check);

	if (model . item . is_type <NPC> ())
	{
		Log :: log (me) << model . item << " will be added to the list of NPCs..." << endl;
		bool check = npcs . add (model . item . to_type <NPC> ());
		assert (check);
		
		Log :: log (me) << model . item << " was added to the list of NPCs." << endl;
	}

	model . set_space (* this);
	
	return true;
}

//	virtual
bool Tile ::
	move (Model & model, Set <Model> & destination)
{
	Log :: trace <Tile> (me, "move", model, destination);
	assert (is_initialized ());
	assert (model . is_initialized ());
	assert (contains (model));
	assert (destination . is_initialized ());
	assert (destination . is_type <Tile> ());

	if (model . item . is_type <NPC> ())
	{
		npcs . drop (model . item . to_type <NPC> ());
	}

	bool check = Set <Model> :: move (model, destination);
	assert (check);

	return true;
}

void Tile ::
	load_xml (TiXmlElement & element)
{
	Log :: trace <Tile> (me, "load_xml", "~element~");
	assert (is_initialized ());

	Log :: log (me) << "element value: " << element . ValueStr () << endl;
	if (element . ValueStr () == string ("include"))
	{
		TiXmlDocument document (tsl_path + "/data/tile/" + element . Attribute ("name") + ".xml");
		load_xml_file (document);
		return;
	}

//	float x = to_float (element . Attribute ("x"));
/*	double x;
	element . Attribute ("x", & x);
	Log :: log (me) << "x: " << x << endl;*/
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
	if (item_xml -> ValueStr () == string ("static_item"))
	{
		item = & Static_Item :: create (name, mesh, volume, mass, solid);
	}
	else if (item_xml -> ValueStr () == string ("npc"))
	{
		item = & NPC :: create (name, mesh, volume, mass);
	}
	else if (item_xml -> ValueStr () == string ("player"))
	{
		item = & Player :: create (name, mesh, volume, mass);
	}
	else
	{
		Log :: error (me) << "Unrecognizable xml tag name: " << item_xml -> ValueStr () << endl;
		abort ();
	}

	Model & model = create_model (* item, position + Ogre :: Vector3 (x, y, z), scale);
	add (model);

	//	TODO re-enable assert ((position + Ogre :: Vector3 (x, y, z) - model . getPosition ()) . length () < 0.01);

	TiXmlElement * material = element . FirstChildElement ("material");
	if (material != NULL)
	{
		model . set_material (material -> Attribute ("name"));
	}
}

void Tile ::
	load_xml_file (TiXmlDocument & document)
{
	Log :: trace <Tile> (me, "load_xml_file", "~document~");
	assert (is_initialized ());

	bool check = document . LoadFile ();
	assert (check);
	assert (! document . Error ());
	TiXmlElement * root = document . RootElement ();

	for (TiXmlElement * model_xml = root -> FirstChildElement ();
				model_xml != NULL; model_xml = model_xml -> NextSiblingElement ())
	{
		load_xml (* model_xml);
	}
	Log :: log (me) << "Tile loaded" << endl;
}

Model & Tile ::
	create_model (Item & item, Ogre :: Vector3 position, float scale)
{
	Log :: trace <Tile> (me, "create_model", item, to_string (position), to_string (scale));
	OgreOde :: Geometry & geometry = item . create_geometry ();
	OgreOde :: Body * body = geometry . getBody ();
	if (body == NULL)
	{
		return * (new Model (item, position, scale, geometry));
	}
	else
	{
		return * (new Movable_Model (item, position, scale, geometry, * body));
	}
}
