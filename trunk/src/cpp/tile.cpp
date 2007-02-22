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

	trace () << "Tile (~new_world~, (" << to_string (new_coordinates . first) << ", " << to_string (new_coordinates . second) << "), " << tsl_path << ")" << endl;
	
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
	for (TiXmlElement * representation_xml = root -> FirstChildElement ("model");
				representation_xml != NULL; representation_xml = representation_xml -> NextSiblingElement ("model"))
	{
		add_xml (* representation_xml);
	}

/*	#ifndef TSL_TRACE
		//	not textured
		represent (Item :: create ("House", "house.mesh", 0, 0, false), 42, 0, 18, 0.016);
		represent (Item :: create ("Wagon", "wagon.mesh", 0, 0), - 10, 0, - 10, 0.0008);
		represent (Item :: create ("Pot", "pot.mesh", 0, 0), - 12, 0, - 10, 0.0008);
		represent (Item :: create ("Pot 2", "pot_2.mesh", 0, 0), - 14, 0, - 10, 0.0008);
		represent (Item :: create ("Pine tree", "pine_tree_2.mesh", 0, 0, false), - 18, 1.4, - 18, 0.06);

		//	forest of 1961 trees
		//	(30000 trees takes to long to load)
		Item * temp_tree = NULL;
		for (int i = - 25; i <= 25; i ++)
		{
			for (int j = int (- sqrt (625 - i * i)); j <= sqrt (625 - i * i); j ++)
			{
				if (0.4 < Ogre :: Math :: RangeRandom (0, 1))
				{
					temp_tree = & represent
					(
						Item :: create
						(
							"Tree no. " + to_string (50 * i + j),
							"tree.mesh",
							0,
							0,
							false
						),
						- 50 + 4 * i + Ogre :: Math :: RangeRandom (- 130, 130),
						0,
						110 + 4 * j + Ogre :: Math :: RangeRandom (- 130, 130),
						0.06
					);
					temp_tree -> get_representation () . turn (- Ogre :: Math :: HALF_PI, Ogre :: Vector3 (1, 0, 0));
				}
				else
				{
					represent
					(
						Item :: create
						(
							"Tree no. " + to_string (50 * i + j),
							"pine_tree.mesh",
							0,
							0,
							false
						),
						- 50 + 4 * i + Ogre :: Math :: RangeRandom (- 130, 130),
						3.84,
						110 + 4 * j + Ogre :: Math :: RangeRandom (- 130, 130),
						0.06
					);
				}
			}
		}
	#endif*/

	assert (is_initialized ());
}

Tile ::
	~Tile ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
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
	assert (! item . has_representation ());

	bool check = Disjoint_Set <Item> :: add (item);
	assert (check);

	item . add_representation (world);
	
	if (item . is_type <NPC> ())
	{
		trace () << item << " will be added to the list of NPCs..." << endl;
		bool check = npcs . insert (& item . to_type <NPC> ()) . second;
		assert (check);
		
		trace () << item << " was added to the list of NPCs." << endl;
	}
	
	return true;
}

//	virtual
bool Tile ::
	move (Item & item, Disjoint_Set <Item> & destination)
{
	trace () << "move (" << item << ", " << destination << ")" << endl;
	assert (is_initialized ());
	assert (item . is_initialized ());
	assert (item . has_representation ());
	assert (destination . is_initialized ());

	Ogre :: Vector3 position = item . get_representation () . getPosition ();
	Ogre :: Quaternion orientation = item . get_representation () . getOrientation ();
	
	item . remove_representation ();

	if (item . is_type <NPC> ())
	{
		npcs . erase (& item . to_type <NPC> ());
	}

	bool check = Disjoint_Set <Item> :: move (item, destination);
	assert (check);

	item . get_representation () . setPosition (position);
	item . get_representation () . setOrientation (orientation);

	return true;
}

Representation & Tile ::
	represent (Item & item, float x, float y, float z, float scale)
{
	trace () << "represent (" << item << ", " << x << ", " << y << ", " << z << ", " << scale << ")" << endl;
	assert (is_initialized ());

	item . append (" (" + * this + ")");
	
	add (item);

	Representation & result = item . get_representation ();
	
	result . setPosition (position + Ogre :: Vector3 (x, y, z));
	result . set_scale (scale);

	return result;
}

void Tile ::
	add_xml (TiXmlElement & element)
{
	trace () << "add_xml (~element~)" << endl;
	assert (is_initialized ());

	float x = to_float (element . Attribute ("x"));
	float y = to_float (element . Attribute ("y"));
	float z = to_float (element . Attribute ("z"));
	float scale = to_float (element . Attribute ("scale"));

	TiXmlElement * item_xml = element . FirstChildElement ();
	//	I'm not sure why 'string' is necessairy here. --Tinus
	assert ((item_xml -> Value () == string ("item")) || (item_xml -> Value () == string ("npc")));

	string name = item_xml -> Attribute ("name");
	string mesh = item_xml -> Attribute ("mesh");
	float volume = to_float (item_xml -> Attribute ("volume"));
	float mass = to_float (item_xml -> Attribute ("mass"));
	bool mobile = item_xml -> Attribute ("mobile") == "true";
	//	bool solid = item_xml -> Attribute ("solid") == "true";
	//	bool visible = item_xml -> Attribute ("visible") == "true";

	Item * item = NULL;
	if (item_xml -> Value () == string ("item"))
	{
		item = & Item :: create (name, mesh, volume, mass, mobile);
	}
	else if (item_xml -> Value () == string ("npc"))
	{
		item = & NPC :: create (name, mesh, volume, mass);
	}
	else
	{
		error () << "didn't recognize xml tag name: " << item_xml -> Value () << endl;
		abort ();
	}
	Representation & representation = represent (* item, x, y, z, scale);

	TiXmlElement * material = element . FirstChildElement ("material");
	if (material != NULL)
	{
		representation . get_entity () . setMaterialName
									(material -> Attribute ("name"));
	}

	TiXmlElement * gravity = element . FirstChildElement ("gravity");
	if (gravity != NULL)
	{
		representation . setAffectedByGravity
						(gravity -> Attribute ("affected") == "true");
	}
}