#include "tile.hpp"

using namespace std;
using namespace tsl;

const int Tile :: side_length (20);

Tile ::
	Tile (OgreOde :: World & new_world, Ogre :: Vector3 new_position, string tsl_path) :
	Object (tsl_path + "/data/tile/tile_" + to_string (new_position . x) + "_" + to_string (new_position . z) + ".xml"),
	position (new_position),
	world (new_world),
	doc (new TiXmlDocument (string :: c_str ()))
{

	trace () << "Tile (~new_world~, " << to_string (position)<< ", " << tsl_path << ")" << endl;
	
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
	for (TiXmlElement * model = root -> FirstChildElement ("model");
				model != NULL; model = model -> NextSiblingElement ("model"))
	{
		TiXmlElement * item = model -> FirstChildElement ("item");
		if (item != NULL)
		{
			string name = item -> Attribute ("name");
			string mesh = item -> Attribute ("mesh");
			float volume = to_float (item -> Attribute ("volume"));
			float mass = to_float (item -> Attribute ("mass"));
			bool mobile = item -> Attribute ("mobile") == "true";
			float x = to_float (model -> Attribute ("x"));
			float y = to_float (model -> Attribute ("y"));
			float z = to_float (model -> Attribute ("z"));
			float scale = to_float (model -> Attribute ("scale"));

			Representation & representation = represent
			(
				Item :: create
				(
					name, mesh, volume, mass, mobile
//					,
//					item -> Attribute ("solid") == "true",
//					item -> Attribute ("visible") == "true"
				),
				x, y, z, scale
			);

			TiXmlElement * material = model -> FirstChildElement ("material");
			if (material != NULL)
			{
				representation . get_entity () . setMaterialName
											(material -> Attribute ("name"));
			}
			
			TiXmlElement * gravity = model -> FirstChildElement ("gravity");
			if (gravity != NULL)
			{
				representation . setAffectedByGravity
								(gravity -> Attribute ("affected") == "true");
			}
		}
	}

	//	the player
	if (! Player :: is_instantiated ())
	{
		represent (Player :: create ("Player", "ninja.mesh", 80, 65), 0.5, 0, 0.5, 0.004);
		assert (Player :: get () . has_representation ());
		assert (contains (Player :: get ()));
	}

	represent (NPC :: create ("Ninja (" + * this + ")", "ninja.mesh", 80, 65), 4.8, 0, 1.2, 0.004);

	#ifndef TSL_TRACE
		//	not textured
		represent (Item :: create ("House", "house.mesh", 0, 0, false), 32, 0, 8, 0.016);
		represent (Item :: create ("Wagon", "wagon.mesh", 0, 0), - 20, 0, - 20, 0.0008);
		represent (Item :: create ("Pot", "pot.mesh", 0, 0), - 22, 0, - 20, 0.0008);
		represent (Item :: create ("Pot 2", "pot_2.mesh", 0, 0), - 24, 0, - 20, 0.0008);
		represent (Item :: create ("Pine tree", "pine_tree_2.mesh", 0, 0, false), - 28, 1.4, - 28, 0.06);

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
						- 60 + 4 * i + Ogre :: Math :: RangeRandom (- 130, 130),
						0,
						120 + 4 * j + Ogre :: Math :: RangeRandom (- 130, 130),
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
						- 60 + 4 * i + Ogre :: Math :: RangeRandom (- 130, 130),
						3.84,
						120 + 4 * j + Ogre :: Math :: RangeRandom (- 130, 130),
						0.06
					);
				}
			}
		}
	#endif

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

	trace () << "represent (" << item << ", " << x << ", " << y << ", " << z << ", " << scale << ") A" << endl;
	item . append (" (" + * this + ")");
	
	trace () << "represent (" << item << ", " << x << ", " << y << ", " << z << ", " << scale << ") B" << endl;
	add (item);

	trace () << "represent (" << item << ", " << x << ", " << y << ", " << z << ", " << scale << ") C" << endl;

	Representation & result = item . get_representation ();
	
	trace () << "represent (" << item << ", " << x << ", " << y << ", " << z << ", " << scale << ") D" << endl;

	result . setPosition
		(position	//	position of the tile
		+ Ogre :: Vector3 (x, y, z)
		+ side_length * Ogre :: Vector3 (0.5, 0, 0.5));
	result . set_scale (scale);

	return result;
}
